#include <iostream>
//ASCII Arts
namespace DCSArt {
// ASCII Art logo. Jazmine font
void header() {
  std::cout << R"(
.oPYo. o     o ooooo  .oPYo. .oPYo.        8               
8.     8     8   8    8    8 8    8        8               
`boo   8     8   8   o8YooP' 8        .oPYo8 .oPYo. .oPYo. 
.P     8     8   8    8      8        8    8 8    ' Yb..   
8      8     8   8    8      8    8   8    8 8    .   'Yb. 
`YooP' 8oooo 8   8    8      `YooP'   `YooP' `YooP' `YooP'
)" << std::endl;
}
} // namespace DCSArt