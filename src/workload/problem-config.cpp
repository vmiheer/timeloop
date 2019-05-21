/* Copyright (c) 2018, NVIDIA CORPORATION. All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *  * Neither the name of NVIDIA CORPORATION nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <vector>
#include <list>

#include "loop-analysis/point-set.hpp"

#include "problem-config.hpp"
#include "workload-config.hpp"
#include "operation-space.hpp"

namespace problem
{

// ======================================== //
//              Problem Shape               //
// ======================================== //

// Globals.
unsigned NumDimensions;
std::map<DimensionID, std::string> DimensionIDToName;
std::map<char, DimensionID> DimensionNameToID;

unsigned NumCoefficients;
std::map<CoefficientID, std::string> CoefficientIDToName;
std::map<std::string, CoefficientID> CoefficientNameToID;
Coefficients DefaultCoefficients;

unsigned NumDataSpaces;
std::map<DataSpaceID, std::string> DataSpaceIDToName;
std::map<std::string, DataSpaceID> DataSpaceNameToID;
std::vector<unsigned> DataSpaceOrder;

std::function<bool(const DataSpaceID d)> IsReadWriteDataSpace;

std::vector<Projection> Projections;

// API.
void ParseProblemShape()
{
  // Dimensions.
  NumDimensions = 7;

  DimensionIDToName = {{0, "R"},
                   {1, "S"},
                   {2, "P"},
                   {3, "Q"},
                   {4, "C"},
                   {5, "K"},
                   {6, "N"}, };

  DimensionNameToID = {{'R', 0 },
                 {'S', 1 },
                 {'P', 2 },
                 {'Q', 3 },
                 {'C', 4 },
                 {'K', 5 },
                 {'N', 6 }, };

  // Coefficients.
  NumCoefficients = 4;

  CoefficientIDToName = {
    { 0, "Wstride" },
    { 1, "Hstride" },
    { 2, "Wdilation" },
    { 3, "Hdilation" }};

  CoefficientNameToID = {
    { "Wstride", 0 },
    { "Hstride", 1 },
    { "Wdilation", 2 },
    { "Hdilation", 3 }};

  DefaultCoefficients = {
    { 0, 1 },
    { 1, 1 },
    { 2, 1 },
    { 3, 1 }};

  // Data Spaces.
  NumDataSpaces = 3;
  
  DataSpaceIDToName = {
    {0, "Weights"},
    {1, "Inputs"},
    {2, "Outputs"},
    {3, "Shared/Illegal"}};

  DataSpaceNameToID = {
    {"Weights", 0},
    {"Inputs", 1},
    {"Outputs", 2},
    {"Shared/Illegal", 3}};

  DataSpaceOrder = { 4, // Weight
                     4, // Input
                     4 }; // Output
  
  IsReadWriteDataSpace = [](const DataSpaceID d) -> bool
    { return d == 2; }; // Output

  Projections.resize(NumDataSpaces);

  // Weights
  Projections[0].resize(DataSpaceOrder[0]);
  Projections[0][0] = {{ NumCoefficients, 0 }}; // 1 * R
  Projections[0][1] = {{ NumCoefficients, 1 }}; // 1 * S
  Projections[0][2] = {{ NumCoefficients, 4 }}; // 1 * C
  Projections[0][3] = {{ NumCoefficients, 5 }}; // 1 * K

  // Inputs
  Projections[1].resize(DataSpaceOrder[1]);
  Projections[1][0] = {{ 2, 0 }, { 0, 2 }}; // Wdilation*R + Wstride*P
  Projections[1][1] = {{ 3, 1 }, { 1, 3 }}; // Hdilation*S + Hstride*Q
  Projections[1][2] = {{ NumCoefficients, 4 }}; // 1 * C
  Projections[1][3] = {{ NumCoefficients, 6 }}; // 1 * N
  
  // Outputs
  Projections[2].resize(DataSpaceOrder[2]);
  Projections[2][0] = {{ NumCoefficients, 2 }}; // 1 * P
  Projections[2][1] = {{ NumCoefficients, 3 }}; // 1 * Q
  Projections[2][2] = {{ NumCoefficients, 5 }}; // 1 * K
  Projections[2][3] = {{ NumCoefficients, 6 }}; // 1 * N
}

}  // namespace problem
