#include <vector>
#include "settings.h"
#include "Calculator.h"
#include "Operations/operations.h"

int main()
{
    Calculator c(operations, operators);

    do
    {
        try
        {
            auto result = c.execute(std::cin);
            if (result)
            {
                std::cout << "-->" << std::endl;
                result->print(std::cout);
                std::cout << std::endl;
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
            while (std::cin.get() != END_COMMAND && std::cin)
                ;
        }
    } while (!(std::cin.eof()));

    std::cout
        << std::endl;
    c.print(std::cout);
    return EXIT_SUCCESS;
}