#include <memory>
#include "../Operation.h"

class Parameters
{
public:
  std::unique_ptr<Matrix> param1 = nullptr;
  std::unique_ptr<Matrix> param2 = nullptr;
  std::string param_str;
  //! int sum_of_param
  Parameters() {}
  Parameters(std::unique_ptr<Matrix> p1,
             std::unique_ptr<Matrix> p2)
      : param1(std::move(p1)),
        param2(std::move(p2)) {}
  Parameters(std::unique_ptr<Matrix> p1,
             std::string p2)
      : param1(std::move(p1)),
        param_str(p2) {}
  Parameters(std::unique_ptr<Matrix> p1)
      : param1(std::move(p1)) {}
  Parameters(std::string p1)
      : param_str(p1) {}
};