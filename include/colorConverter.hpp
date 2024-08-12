// Copyright (c) 2014, Jan Winkler <winkler@cs.uni-bremen.de>
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of Universität Bremen nor the names of its
//       contributors may be used to endorse or promote products derived from
//       this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

/* Author: Jan Winkler */


#include <iostream>
#include <cstdlib>
#include <cmath>
#include <immintrin.h>
#include <xmmintrin.h>
#pragma once

//using namespace std;


/*! \brief Convert RGB to HSV color space
  
  Converts a given set of RGB values `r', `g', `b' into HSV
  coordinates. The input RGB values are in the range [0, 1], and the
  output HSV values are in the ranges h = [0, 360], and s, v = [0,
  1], respectively.
  
  \param fR Red component, used as input, range: [0, 1]
  \param fG Green component, used as input, range: [0, 1]
  \param fB Blue component, used as input, range: [0, 1]
  \param fH Hue component, used as output, range: [0, 360]
  \param fS Hue component, used as output, range: [0, 1]
  \param fV Hue component, used as output, range: [0, 1]
  
*/
void RGBtoHSV(float& fR, float& fG, float fB, float& fH, float& fS, float& fV) {
  float fCMax = std::max(std::max(fR, fG), fB);
  float fCMin = std::min(std::min(fR, fG), fB);
  float fDelta = fCMax - fCMin;
  
  if(fDelta > 0) {
    if(fCMax == fR) {
      fH = 60 * (fmod(((fG - fB) / fDelta), 6));
    } else if(fCMax == fG) {
      fH = 60 * (((fB - fR) / fDelta) + 2);
    } else if(fCMax == fB) {
      fH = 60 * (((fR - fG) / fDelta) + 4);
    }
    
    if(fCMax > 0) {
      fS = fDelta / fCMax;
    } else {
      fS = 0;
    }
    
    fV = fCMax;
  } else {
    fH = 0;
    fS = 0;
    fV = fCMax;
  }
  
  if(fH < 0) {
    fH = 360 + fH;
  }
}


/*! \brief Convert HSV to RGB color space
  
  Converts a given set of HSV values `h', `s', `v' into RGB
  coordinates. The output RGB values are in the range [0, 1], and
  the input HSV values are in the ranges h = [0, 360], and s, v =
  [0, 1], respectively.
  
  \param fR Red component, used as output, range: [0, 1]
  \param fG Green component, used as output, range: [0, 1]
  \param fB Blue component, used as output, range: [0, 1]
  \param fH Hue component, used as input, range: [0, 360]
  \param fS Hue component, used as input, range: [0, 1]
  \param fV Hue component, used as input, range: [0, 1]
  
*/

void HSVtoRGB(float &fR, float &fG, float &fB, float &fH, float &fS, float &fV) {
  float fC = fV * fS; // Chroma
  float fHPrime = fmod(fH / 60.0, 6);

 float fX = fC * (1 - fabs(fmod(fHPrime, 2) - 1));
 //std::cout << "fMod: " << fabs(fmod(fHPrime, 2) - 1) << std::endl;


  float fM = fV - fC;
 
  
  
  if(0 <= fHPrime && fHPrime < 1) {
    fR = fC;
    fG = fX;
    fB = 0;
  } else if(1 <= fHPrime && fHPrime < 2) {
    fR = fX;
    fG = fC;
    fB = 0;
  } else if(2 <= fHPrime && fHPrime < 3) {
    fR = 0;
    fG = fC;
    fB = fX;
  } else if(3 <= fHPrime && fHPrime < 4) {
    fR = 0;
    fG = fX;
    fB = fC;
  } else if(4 <= fHPrime && fHPrime < 5) {
    fR = fX;
    fG = 0;
    fB = fC;
  } else if(5 <= fHPrime && fHPrime < 6) {
    fR = fC;
    fG = 0;
    fB = fX;
  } else {
    fR = 0;
    fG = 0;
    fB = 0;
  }
  
  fR += fM;
  fG += fM;
  fB += fM;

  
}


//Assuming saturation and value are both 1. Skips some computations
void HSVtoRGBConstSV(float &fR, float &fG, float &fB, float &fH) {
  float fHPrime = fmod(fH / 60.0, 6);
  float fX = (1 - fabs(fmod(fHPrime, 2) - 1));
  

  
  if(0 <= fHPrime && fHPrime < 1) {
    fR = 1.0;
    fG = fX;
    fB = 0;
  } else if(1 <= fHPrime && fHPrime < 2) {
    fR = fX;
    fG = 1.0;
    fB = 0;
  } else if(2 <= fHPrime && fHPrime < 3) {
    fR = 0;
    fG = 1.0;
    fB = fX;
  } else if(3 <= fHPrime && fHPrime < 4) {
    fR = 0;
    fG = fX;
    fB = 1.0;
  } else if(4 <= fHPrime && fHPrime < 5) {
    fR = fX;
    fG = 0;
    fB = 1.0;
  } else if(5 <= fHPrime && fHPrime < 6) {
    fR = 1.0;
    fG = 0;
    fB = fX;
  } else {
    fR = 0;
    fG = 0;
    fB = 0;
  }
  
  
}



void HSVtoRGBSIMD(__m256 &fR, __m256 &fG, __m256 &fB, __m256 &fH, __m256 &fS, __m256 &fV) {
  // float fC = fV * fS; // Chroma
  __m256 fC = _mm256_mul_ps(fV, fS);
  // float fHPrime = fmod(fH / 60.0, 6);
  __m256 numer = _mm256_div_ps(fH, _mm256_set1_ps(60.0)); //fH / 60.0
  __m256 fHPrime = _mm256_sub_ps(numer, _mm256_mul_ps(_mm256_round_ps(_mm256_div_ps(numer, _mm256_set1_ps(6.0)), _MM_FROUND_TO_ZERO), _mm256_set1_ps(6.0))); //fmod(fH / 60.0, 6.0)

  // float fX = fC * (1 - fabs(fmod(fHPrime, 2) - 1));
  __m256 fModPrime =  _mm256_sub_ps(fHPrime, _mm256_mul_ps(_mm256_round_ps(_mm256_div_ps(fHPrime, _mm256_set1_ps(2.0)), _MM_FROUND_TO_ZERO), _mm256_set1_ps(2.0))); //fmod(fHPrime, 2)
 
  fModPrime = _mm256_sub_ps(fModPrime, _mm256_set1_ps(1.0));
   
  fModPrime =   _mm256_andnot_ps(_mm256_set1_ps(-0.0f), fModPrime); //fabs
  fModPrime = _mm256_sub_ps(_mm256_set1_ps(1.0), fModPrime);
  
  __m256 fX = _mm256_mul_ps(fC, fModPrime);
 

  // float fM = fV - fC;
  __m256 fM = _mm256_sub_ps(fV, fC);
  
  __m256 mask1 = _mm256_and_ps(_mm256_cmp_ps(fHPrime, _mm256_set1_ps(0.0), _CMP_GE_OQ), _mm256_cmp_ps(fHPrime, _mm256_set1_ps(1.0), _CMP_LT_OQ));
  __m256 mask2 = _mm256_and_ps(_mm256_cmp_ps(fHPrime, _mm256_set1_ps(1.0), _CMP_GE_OQ), _mm256_cmp_ps(fHPrime, _mm256_set1_ps(2.0), _CMP_LT_OQ));
  __m256 mask3 = _mm256_and_ps(_mm256_cmp_ps(fHPrime, _mm256_set1_ps(2.0), _CMP_GE_OQ), _mm256_cmp_ps(fHPrime, _mm256_set1_ps(3.0), _CMP_LT_OQ));
  __m256 mask4 = _mm256_and_ps(_mm256_cmp_ps(fHPrime, _mm256_set1_ps(3.0), _CMP_GE_OQ), _mm256_cmp_ps(fHPrime, _mm256_set1_ps(4.0), _CMP_LT_OQ));
  __m256 mask5 = _mm256_and_ps(_mm256_cmp_ps(fHPrime, _mm256_set1_ps(4.0), _CMP_GE_OQ), _mm256_cmp_ps(fHPrime, _mm256_set1_ps(5.0), _CMP_LT_OQ));
  __m256 mask6 = _mm256_and_ps(_mm256_cmp_ps(fHPrime, _mm256_set1_ps(5.0), _CMP_GE_OQ), _mm256_cmp_ps(fHPrime, _mm256_set1_ps(6.0), _CMP_LT_OQ));
  __m256 mask_none = _mm256_or_ps(_mm256_cmp_ps(fHPrime, _mm256_set1_ps(0.0), _CMP_LT_OQ), _mm256_cmp_ps(fHPrime, _mm256_set1_ps(6.0), _CMP_GE_OQ));
 
 
 
   // if(0 <= fHPrime && fHPrime < 1) {
  //   fR = fC;
  //   fG = fX;
  //   fB = 0;
  // } 

  fR = _mm256_blendv_ps(fR, fC, mask1);
  fG = _mm256_blendv_ps(fG, fX, mask1);
  fB = _mm256_blendv_ps(fB, _mm256_set1_ps(0.0), mask1);


  //else if(1 <= fHPrime && fHPrime < 2) {
  //   fR = fX;
  //   fG = fC;
  //   fB = 0;
  fR = _mm256_blendv_ps(fR, fX, mask2);
  fG = _mm256_blendv_ps(fG, fC, mask2);
  fB = _mm256_blendv_ps(fB, _mm256_set1_ps(0.0), mask2);

  // } else if(2 <= fHPrime && fHPrime < 3) {
  //   fR = 0;
  //   fG = fC;
  //   fB = fX;
  fR = _mm256_blendv_ps(fR, _mm256_set1_ps(0.0) , mask3);
  fG = _mm256_blendv_ps(fG, fC, mask3);
  fB = _mm256_blendv_ps(fB, fX, mask3);
  // } else if(3 <= fHPrime && fHPrime < 4) {
  //   fR = 0;
  //   fG = fX;
  //   fB = fC;
  fR = _mm256_blendv_ps(fR, _mm256_set1_ps(0.0) , mask4);
  fG = _mm256_blendv_ps(fG, fX, mask4);
  fB = _mm256_blendv_ps(fB, fC, mask4);
  // } else if(4 <= fHPrime && fHPrime < 5) {
  //   fR = fX;
  //   fG = 0;
  //   fB = fC;
  fR = _mm256_blendv_ps(fR, fX , mask5);
  fG = _mm256_blendv_ps(fG, _mm256_set1_ps(0.0), mask5);
  fB = _mm256_blendv_ps(fB, fC, mask5);
  // } else if(5 <= fHPrime && fHPrime < 6) {
  //   fR = fC;
  //   fG = 0;
  //   fB = fX;
  fR = _mm256_blendv_ps(fR, fC , mask6);
  fG = _mm256_blendv_ps(fG, _mm256_set1_ps(0.0), mask6);
  fB = _mm256_blendv_ps(fB, fX, mask6);
  // } else {
  //   fR = 0;
  //   fG = 0;
  //   fB = 0;
  // }
  fR = _mm256_blendv_ps(fR, _mm256_set1_ps(0.0), mask_none);
  fG = _mm256_blendv_ps(fG, _mm256_set1_ps(0.0), mask_none);
  fB = _mm256_blendv_ps(fB, _mm256_set1_ps(0.0), mask_none);
  
  fR = _mm256_add_ps(fR, fM);
  fG = _mm256_add_ps(fG, fM);
  fB = _mm256_add_ps(fB, fM);



  // fR += fM;
  // fG += fM;
  // fB += fM;
}