#include <memory>
#include "../Operation.h"

class Parameters
{
public:
  std::unique_ptr<Matrix> param1 = nullptr;
  std::unique_ptr<Matrix> param2 = nullptr;
  std::unique_ptr<Matrix> param3 = nullptr;
  std::string param_str;
  Parameters(std::unique_ptr<Matrix> p1 = nullptr,
             std::unique_ptr<Matrix> p2 = nullptr,
             std::unique_ptr<Matrix> p3 = nullptr)
      : param1(std::move(p1)),
        param2(std::move(p2)),
        param3(std::move(p3)) {}
  Parameters(std::unique_ptr<Matrix> p1,
             std::string p2)
      : param1(std::move(p1)),
        param_str(p2) {}
  Parameters(std::string p1)
      : param_str(p1) {}
};