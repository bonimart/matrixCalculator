#pragma once
#include <memory>
#include "../../Matrix/Matrix.h"

/**
 * @brief helper class that can store up to two matrices
 *
 */
class Parameters
{
public:
  std::unique_ptr<Matrix> param1 = nullptr;
  std::unique_ptr<Matrix> param2 = nullptr;
  int paramCount;
  Parameters() { paramCount = 0; }
  Parameters(std::unique_ptr<Matrix> p1)
      : param1(std::move(p1)) { paramCount = 1; }
  Parameters(std::unique_ptr<Matrix> p1,
             std::unique_ptr<Matrix> p2)
      : param1(std::move(p1)),
        param2(std::move(p2)) { paramCount = 2; }
};