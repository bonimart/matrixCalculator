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
            auto result = c.execute(std::cin, std::cout);
            // print result of an operation
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
            // skip all unnecessary characters after an exception is thrown
            while (std::cin.get() != END_COMMAND && std::cin)
                ;
        }
    } while (!(std::cin.eof()));

    return EXIT_SUCCESS;
}
