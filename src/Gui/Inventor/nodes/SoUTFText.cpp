/***************************************************************************
 *   Copyright (c) 2012 Luke Parry <l.parry@warwick.ac.uk>                 *
 *                                                                         *
 *   This file is part of the FreeCAD CAx development system.              *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License as published by the Free Software Foundation; either          *
 *   version 2 of the License, or (at your option) any later version.      *
 *                                                                         *
 *   This library  is distributed in the hope that it will be useful,      *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU Library General Public License for more details.                  *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this library; see the file COPYING.LIB. If not,    *
 *   write to the Free Software Foundation, Inc., 59 Temple Place,         *
 *   Suite 330, Boston, MA  02111-1307, USA                                *
 *                                                                         *
 ***************************************************************************/

#include "PreCompiled.h"

#ifndef _PreComp_
#include <string.h>
#include <float.h>
#include <list>
#endif

#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>
#include <Inventor/bundles/SoMaterialBundle.h>
#include <Inventor/details/SoTextDetail.h>
#include <Inventor/elements/SoFontNameElement.h>
#include <Inventor/elements/SoFontSizeElement.h>
#include <Inventor/elements/SoGLMultiTextureEnabledElement.h>
#include <Inventor/elements/SoGLCacheContextElement.h>
#include <Inventor/elements/SoComplexityTypeElement.h>
#include <Inventor/elements/SoComplexityElement.h>
#include <Inventor/elements/SoCacheElement.h>
#include <Inventor/elements/SoTextOutlineEnabledElement.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/misc/SoGlyph.h>
#include <Inventor/system/gl.h>
#include <Inventor/threads/SbMutex.h>

#include "SoUTFText.h"

// *************************************************************************

SO_NODE_SOURCE(SoUTFText);

// *************************************************************************

/*!
  Constructor.
*/
SoUTFText::SoUTFText(void)
{
  SO_NODE_CONSTRUCTOR(SoUTFText);

  SO_NODE_ADD_FIELD(string, (""));
  SO_NODE_ADD_FIELD(spacing, (1.0f));
  SO_NODE_ADD_FIELD(justification, (SoUTFText::LEFT));
  SO_NODE_ADD_FIELD(width, (0.0f));

  SO_NODE_DEFINE_ENUM_VALUE(Justification, LEFT);
  SO_NODE_DEFINE_ENUM_VALUE(Justification, RIGHT);
  SO_NODE_DEFINE_ENUM_VALUE(Justification, CENTER);
  SO_NODE_SET_SF_ENUM_TYPE(justification, Justification);
}

/*!
  Destructor.
*/
SoUTFText::~SoUTFText()
{
    invalidateCache();
}

// Doc in parent.
void SoUTFText::initClass(void)
{
    SO_NODE_INIT_CLASS(SoUTFText, SoShape, "Shape");
}

void SoUTFText::invalidateCache() {
    cache = false;
}

// Doc in parent.
void SoUTFText::GLRender(SoGLRenderAction * action)
{
    if (!this->shouldGLRender(action))
        return;

    SoState * state = action->getState();

    static SbBool warned = FALSE;
    if (!warned) {
      const int stackidx = SoTextOutlineEnabledElement::getClassStackIndex();
      const SbBool outlinepresence = state->isElementEnabled(stackidx);

      if (outlinepresence && SoTextOutlineEnabledElement::get(state)) {
        SoDebugError::postWarning("SoUTFText::GLRender",
                                  "Support for rendering SoUTFText nodes in outline "
                                  "(i.e. heeding the SoTextOutlineEnabledElement) "
                                  "not yet implemented.");
        warned = TRUE;
      }
    }


  setUpGlyphs(state);
  SbName fontName = SoFontNameElement::get(state);
  float fontSize  = SoFontSizeElement::get(state);

  SbBool do2Dtextures = FALSE;
  SbBool do3Dtextures = FALSE;
  if (SoGLMultiTextureEnabledElement::get(state, 0)) {
    do2Dtextures = TRUE;
    if (SoGLMultiTextureEnabledElement::getMode(state, 0) == SoGLMultiTextureEnabledElement::TEXTURE3D) {
      do3Dtextures = TRUE;
    }
  }

  SoMaterialBundle mb(action);
  mb.sendFirst();

  glBegin(GL_TRIANGLES);
  glNormal3f(0.0f, 0.0f, 1.0f);

  float ypos = 0.0f;
  int i;
  int n = this->string.getNum();

  for (i = 0; i < n; i++) {
    float stretchfactor, stretchlength;
    calculateStringStretch(i, stretchfactor, stretchlength, state);

    float xpos = 0.0f;
    const float currwidth = stretchlength;
    switch (this->justification.getValue()) {
      case SoUTFText::RIGHT:
        xpos = -currwidth;
        break;
      case SoUTFText::CENTER:
        xpos = -currwidth * 0.5f;
        break;
      default:
        break;
    }

    SbUTFString str = this->string[i];
    SoGlyph * prevGlyph = 0;

    UTFString utfString = str.getUTFString();

    for(std::vector<unsigned int>::const_iterator it = utfString.begin(); it != utfString.end(); ++it)
    {
        unsigned int currentChar = *it;
        const SoGlyph *glyph = SoGlyph::getGlyph(state, *it, SbVec2s(0,0), 0);

        // Get kerning
        if (currentChar > 0 && prevGlyph) {
            float kerningx, kerningy;
            SbVec2s kerning = glyph->getKerning(*prevGlyph);
            xpos += kerning[0] * stretchfactor * fontSize;
        }

        prevGlyph = const_cast<SoGlyph*>(glyph);

        const SbVec2f * coords = glyph->getCoords();
        const int * ptr = glyph->getFaceIndices();

        while (*ptr >= 0) {
            SbVec2f v0, v1, v2;
            v2 = coords[*ptr++];
            v1 = coords[*ptr++];
            v0 = coords[*ptr++];

            // FIXME: Is the text textured correctly when stretching is
            // applied (when width values have been given that are
            // not the same as the length of the string)? jornskaa 20040716
            if (do2Dtextures) {
                glTexCoord2f(v0[0] + xpos/fontSize, v0[1] + ypos/fontSize);
            }
            glVertex3f(v0[0] * fontSize + xpos, v0[1] * fontSize + ypos, 0.0f);

            if (do2Dtextures) {
                glTexCoord2f(v1[0] + xpos/fontSize, v1[1] + ypos/fontSize);
            }
            glVertex3f(v1[0] * fontSize + xpos, v1[1] * fontSize + ypos, 0.0f);

            if (do2Dtextures) {
                glTexCoord2f(v2[0] + xpos/fontSize, v2[1] + ypos/fontSize);
            }
            glVertex3f(v2[0] * fontSize + xpos, v2[1] * fontSize + ypos, 0.0f);

        }

        SbVec2s advance = glyph->getAdvance();
        xpos += (advance[0] * stretchfactor * fontSize);
    }

    ypos -= fontSize * this->spacing.getValue();
  }
  glEnd();


  if (SoComplexityTypeElement::get(state) == SoComplexityTypeElement::OBJECT_SPACE) {
    SoGLCacheContextElement::shouldAutoCache(state, SoGLCacheContextElement::DO_AUTO_CACHE);
    SoGLCacheContextElement::incNumShapes(state);
  }
}

// Doc in parent.
void SoUTFText::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
    SoState * state = action->getState();
    SbName fontName = SoFontNameElement::get(state);
    float fontSize = SoFontSizeElement::get(state);

    // don't create an new cache in getPrimitiveCount(). SoCacheElement is not enabled
    if (action->is3DTextCountedAsTriangles() && !this->cacheValid()) {

        const int lines = this->string.getNum();
        int numtris = 0;
        for (int i = 0;i < lines; ++i) {

            SbUTFString str = this->string[i];

            UTFString utfString = str.getUTFString();

            for(std::vector<unsigned int>::const_iterator it = utfString.begin(); it != utfString.end(); ++it)
            {
                const SoGlyph *glyph = SoGlyph::getGlyph(state, *it, SbVec2s(0,0), 0);
                int cnt = 0;
                const int * ptr = glyph->getFaceIndices();
                while (*ptr++ >= 0)
                    cnt++;

                numtris += cnt / 3;
            }
        }
        action->addNumTriangles(numtris);
    }
    else {
        action->addNumText(this->string.getNum());
    }
}


void SoUTFText::calculateStringStretch(const int i, float & stretchfactor, float & stretchedlength, SoState *state)
{
    assert(i < this->string.getNum());

    // Some sanitychecking before starting the calculations
    if (i >= this->width.getNum() || this->width[i] <= 0 ||
        this->string[i].getLength() == 0) {

      stretchfactor = 1.0f;
      stretchedlength = this->stringwidths[i];
      return;
    }

    // Approximate the stretchfactor
    stretchfactor = this->width[i] / this->stringwidths[i];

    // Get current font details
    SbName fontName = SoFontNameElement::get(state);
    float fontSize = SoFontSizeElement::get(state);

    SoGlyph *glyph = 0;
    float originalmaxx = 0.0f;
    float originalmaxxpos = 0.0f;
    float originalxpos = 0.0f;
    float maxglyphwidth = 0.0f;
    float maxx = 0.0f;
    unsigned int strcharidx;

    // Find last character in the stretched text
    SbUTFString str = this->string[i];


    UTFString utfString = str.getUTFString();

    SoGlyph *prevGlyph = 0;
    for(std::vector<unsigned int>::const_iterator it = utfString.begin(); it != utfString.end(); ++it)
    {
        unsigned int currentChar = *it;
        const SoGlyph *glyph = SoGlyph::getGlyph(state, *it, SbVec2s(0,0), 0);
        float glyphwidth = glyph->getWidth() * fontSize;

      // Adjust the distance between neighbouring characters
      if (prevGlyph) {
          SbVec2s kerning;
          kerning = glyph->getKerning(*prevGlyph);
          originalxpos += kerning[0] * fontSize;
      }

      // Find the maxmimum endposition in the x-direction
      float endx = originalxpos * stretchfactor + glyphwidth;
      if (endx > maxx) {
          originalmaxxpos = originalxpos;
          originalmaxx = originalxpos + glyphwidth;

          maxx = endx;
          maxglyphwidth = glyphwidth;
      }

      // Advance to the next character in the x-direction
      SbVec2s advance = glyph->getAdvance();
      originalxpos += advance[0] * fontSize;

  //     // Remove the previous glyph from memory
  //     if (prevglyph) {
  //       cc_glyph3d_unref(prevglyph);
  //     }
  //
  //     // Make ready for next run
  //     prevglyph = glyph;
    }

  //   // Unreference the last glyph
  //   cc_glyph3d_unref(prevglyph);
  //   prevglyph = NULL;

    // Calculate the accurate stretchfactor and the width of the
    // string. This should be close to the specified width unless the
    // specified width is less than the longest character in the string.
    const float oldendxpos = this->stringwidths[i] - maxglyphwidth;
    const float newendxpos = this->width[i] - maxglyphwidth;
    if (oldendxpos <= 0 || newendxpos <= 0) {
        // oldendxpos should be > 0 anyways, but just in case.  if
        // newendxpos < 0, then the specified width is less than that
        // possible with the current font.
        stretchfactor = 0.0f;
        stretchedlength = maxglyphwidth;
    }
    else {
        // The width of the stretched string is longer than the longest
        // character in the string.
        stretchfactor = newendxpos / oldendxpos;
        stretchedlength = stretchfactor * originalmaxxpos + maxglyphwidth;
    }
}

// Doc in parent.
void SoUTFText::computeBBox(SoAction * action, SbBox3f & box, SbVec3f & center)
{
    SoState * state = action->getState();

    setUpGlyphs(state);
    SbName fontName = SoFontNameElement::get(state);
    float fontSize = SoFontSizeElement::get(state);

    int i;
    float maxw = FLT_EPSILON;

    for (i = 0; i < this->string.getNum(); i++) {
        float stretchfactor, stretchlength;
        this->calculateStringStretch(i, stretchfactor, stretchlength, state);
        maxw = SbMax(maxw, stretchlength);
    }

    if (maxw == FLT_EPSILON)  // There is no text to bound. Returning.
        return;

    float maxy, miny;
    float minx, maxx;

    minx = 0;
    maxx = maxw;

    miny = -fontSize * this->spacing.getValue() * (this->string.getNum() - 1);
    maxy = fontSize;

    switch (this->justification.getValue()) {
      case SoUTFText::LEFT:
          break;
      case SoUTFText::RIGHT:
          maxx -= maxw;
          minx -= maxw;
          break;
      case SoUTFText::CENTER:
          maxx -= maxw * 0.5f;
          minx -= maxw * 0.5f;
          break;
      default:
          assert(0 && "Unknown justification");
          minx = maxx = 0.0f;
          break;
    }

    box.setBounds(SbVec3f(minx, miny, 0.0f), SbVec3f(maxx, maxy, 0.0f));

    // maxglyphbbox should never be empty here.
    assert(!this->maxglyphbbox.isEmpty());

    // Expanding bbox so that glyphs like 'j's and 'q's are completely inside.
    box.extendBy(SbVec3f(0, this->maxglyphbbox.getMin()[1] - (this->string.getNum() - 1) * fontSize, 0));
    box.extendBy(this->maxglyphbbox);
    center = box.getCenter();

}

// Doc in parent.
void SoUTFText::generatePrimitives(SoAction * action)
{
    SoState * state = action->getState();
    setUpGlyphs(state);

    SbName fontName = SoFontNameElement::get(state);
    float fontSize  = SoFontSizeElement::get(state);

    SbBool do2Dtextures = FALSE;
    SbBool do3Dtextures = FALSE;
    if (SoMultiTextureEnabledElement::get(state, 0)) {
        do2Dtextures = TRUE;
        if (SoMultiTextureEnabledElement::getMode(state,0) ==
            SoMultiTextureEnabledElement::TEXTURE3D) {
            do3Dtextures = TRUE;
        }
    }

    SoPrimitiveVertex vertex;
    SoTextDetail detail;
    detail.setPart(0);
    vertex.setDetail(&detail);
    vertex.setMaterialIndex(0);

    this->beginShape(action, SoShape::TRIANGLES, NULL);
    vertex.setNormal(SbVec3f(0.0f, 0.0f, 1.0f));

    float ypos = 0.0f;
    int i;
    int n = this->string.getNum();
    for (i = 0; i < n; i++) {
      float stretchfactor, stretchlength;
      this->calculateStringStretch(i, stretchfactor, stretchlength, state);

      detail.setStringIndex(i);
      float xpos = 0.0f;
      const float currwidth = stretchlength;
      switch (this->justification.getValue()) {
        case SoUTFText::RIGHT:
            xpos = -currwidth;
            break;
        case SoUTFText::CENTER:
            xpos = - currwidth * 0.5f;
            break;
      }

      SbUTFString str = this->string[i];

      const float * maxbbox;
      const UTFString utfString = str.getUTFString();

      SoGlyph *prevGlyph = 0;
      for(std::vector<unsigned int>::const_iterator it = utfString.begin(); it != utfString.end(); ++it)
      {
          const unsigned int currentChar = *it;
          const SoGlyph *glyph = SoGlyph::getGlyph(state, *it, SbVec2s(0,0), 0);

        // Get kerning
        if (currentChar > 0 && prevGlyph) {
            SbVec2s kerning;
            kerning = glyph->getKerning(*prevGlyph);
            xpos += kerning[0]* stretchfactor * fontSize;
        }

//         if (prevglyph) {
//             cc_glyph3d_unref(prevglyph);
//         }

        prevGlyph = const_cast<SoGlyph*> (glyph);
        detail.setCharacterIndex(currentChar);

        const SbVec2f * coords = glyph->getCoords();
        const int * ptr = glyph->getFaceIndices();

        while (*ptr >= 0) {
            SbVec2f v0, v1, v2;
            v2 = coords[*ptr++];
            v1 = coords[*ptr++];
            v0 = coords[*ptr++];

            if(do2Dtextures) {
              vertex.setTextureCoords(SbVec2f(v0[0] + xpos/fontSize, v0[1] + ypos/fontSize));
            }
            vertex.setPoint(SbVec3f(v0[0] * fontSize + xpos, v0[1] * fontSize + ypos, 0.0f));
            this->shapeVertex(&vertex);

            if(do2Dtextures) {
              vertex.setTextureCoords(SbVec2f(v1[0] + xpos/fontSize, v1[1] + ypos/fontSize));
            }
            vertex.setPoint(SbVec3f(v1[0] * fontSize + xpos, v1[1] * fontSize + ypos, 0.0f));
            this->shapeVertex(&vertex);

            if(do2Dtextures) {
              vertex.setTextureCoords(SbVec2f(v2[0] + xpos/fontSize, v2[1] + ypos/fontSize));
            }
            vertex.setPoint(SbVec3f(v2[0] * fontSize + xpos, v2[1] * fontSize + ypos, 0.0f));
            this->shapeVertex(&vertex);
        }
        SbVec2s advance = glyph->getAdvance();
        xpos += (advance[0] * stretchfactor * fontSize);
      }
      ypos -= fontSize * this->spacing.getValue();
//       if (prevglyph) {
//         cc_glyph3d_unref(prevglyph);
//         prevglyph = NULL;
//       }
    }
    this->endShape();
    cache = true;
}

// doc in parent
SoDetail * SoUTFText::createTriangleDetail(SoRayPickAction *action,
                                           const SoPrimitiveVertex * v1,
                                           const SoPrimitiveVertex * v2,
                                           const SoPrimitiveVertex * v3,
                                           SoPickedPoint *pp)
{
  // generatePrimitives() places text details inside each primitive vertex
  assert(v1->getDetail());
  return v1->getDetail()->copy();
}


void SoUTFText::notify(SoNotList * list)
{
    if (this->cacheValid()) {
        SoField * f = list->getLastField();
        if (f == &this->string) {
            this->invalidateCache();
        }
    }
    inherited::notify(list);
}

// returns "normalized" width of specified string. When too few
// width values are supplied, the glyphwidths are used instead.
float SoUTFText::getWidth(const int idx, const float fontsize)
{
  if (idx < this->width.getNum() && this->width[idx] > 0.0f)
      return this->width[idx] / fontsize;

  return this->glyphwidths[idx];
}

// recalculate glyphs
void SoUTFText::setUpGlyphs(SoState * state)
{
  if (this->cacheValid())
      return;

  this->invalidateCache();
  state->push();

  // Get current font details
  SbName fontName = SoFontNameElement::get(state);
  float fontSize = SoFontSizeElement::get(state);

  // Clear glyph details
  this->glyphwidths.truncate(0);
  this->stringwidths.truncate(0);
  this->maxglyphbbox.makeEmpty();

  SbVec2s kerning;
  SbVec2s advance;

  for (int i = 0; i < this->string.getNum(); i++) {

      float stringWidth = 0.0f;
      SbUTFString str = this->string[i];
      const float * maxbbox;
      const UTFString utfString = str.getUTFString();

      SoGlyph *prevGlyph = 0;
      for(std::vector<unsigned int>::const_iterator it = utfString.begin(); it != utfString.end(); ++it)
      {
          unsigned int currentChar = *it;
          const SoGlyph *glyph = SoGlyph::getGlyph(state, *it, SbVec2s(0,0), 0);

          SbVec2f bbox = glyph->getBoundingBox().getMax();

          this->maxglyphbbox.extendBy(SbVec3f(0, bbox[0] * fontSize, 0));
          this->maxglyphbbox.extendBy(SbVec3f(0, bbox[1] * fontSize, 0));

          this->glyphwidths.append(glyph->getWidth());

          if (currentChar > 0 && prevGlyph)
              kerning = glyph->getKerning(*prevGlyph);
              advance = glyph->getAdvance();

          prevGlyph = const_cast<SoGlyph*>(glyph);
          stringWidth += (advance[0] + kerning[0]) * fontSize;
      }

      if (prevGlyph) {
          // Have to remove the appended advance and add the last character to the calculated with
          stringWidth += (prevGlyph->getWidth() - advance[0]) * fontSize;
          prevGlyph = 0; // To make sure the next line starts with blank sheets
      }

      this->stringwidths.append(stringWidth);

  }

  state->pop();
}

// *************************************************************************