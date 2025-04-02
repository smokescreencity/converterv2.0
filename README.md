一、需求分析
实验选题为《制式转换》。该程序作为一个转换程序，是一个非常实用的小工具，可以为生产生活提供便利，展现信息技术对生活的改善与提升，该程序设计了汉化界面并将相关功能封装到类中，以便于使用和维护。程序主要包含以下功能。
1.设计进制转换：
从十进制到二进制，从十进制到八进制，从十进制到十六进制，从二进制到十进制，从八进制到十进制，从十六进制到十进制的转换程序。
2.设计时间转换：
如北京时间对于世界其他主要城市之间的时间转换。
3.制式转换：
设计从米制到标准制和标准制到米制之间的转换。
二、总体设计
为了实现需求分析中的功能，该函数主要包括五个部分，分别是四个类和主程序部分。通过对不同部分的划分，可以使程序便于理解和维护，也可以针对特定功能，选择特定的部分重新设计，实现程序的精简，让其按需高效发挥作用。以下是对程序的总体设计
（一）类定义：
1.BaseConverter：
这个类提供了将十进制数转换为任意进制（2到16进制）的功能。主要实现方法是decimalToBase，通过不断将十进制数除以目标进制并取余数来完成转换。
2.DecimalConverter：
这个类实现了将任意进制（2到16进制）的数转换为十进制的功能。主要实现方法是baseToDecimal，通过按位读取输入字符串并计算其对应的十进制值。
3.MetricConverter：
这个类提供了长度、重量、体积等常用单位之间的转换方法。包括米转英尺、公里转英里、公斤转磅和升转加仑。
4.TimeZoneConverter：
这个类用于不同时区之间的时间转换。主要包含两个方法：getOffset用于获取指定时区相对于UTC的时区偏移量；convert用于根据时区偏移量转换时间。
（二）主程序：
首先，为了确保中文输出的稳定性，防止不同编码是中文输出产生紊乱，使用SetConsoleOutputCP和SetConsoleCP函数设置控制台的代码页为936，以支持中文显示。
然后进入一个无限循环，显示主菜单并根据用户的选择执行相应的转换操作。
用户可以选择进行十进制到其他进制的转换、其他进制到十进制的转换、度量衡转换以及时区转换。
对于每种转换操作，程序会提示用户输入相应的数据，并调用相应的类和方法进行计算和输出。
在每个转换操作结束后，程序会等待用户按回车键继续，直到用户选择退出（输入0）。
异常处理：
对于无效的转换类型、时区名称和时间输入，程序会给出相应的错误提示，并跳出当前的转换操作，返回主菜单继续等待用户输入。
三、具体实现
（一）首先包含了一些必要的头文件：
iostream：用于输入输出操作。
string：用于处理字符串。
iomanip：用于格式化输出，例如设置小数位数。
windows.h：用于设置控制台支持中文显示。
using namespace std;表示使用标准命名空间，简化了代码中标准库函数和对象的调用。
（二）类 BaseConverter
作用：这一类提供将十进制数转换为任意进制（2到16进制）的功能。
函数 decimalToBase(int decimal, int base)：
该函数接受一个十进制整数 decimal 和一个目标进制 base。
如果输入的 decimal 为0，直接返回字符串"0"。
使用一个布尔变量 isNegative 来判断输入的数字是否为负数，如果是负数，先将其转换为绝对值进行处理。
通过循环的方式，将十进制数不断除以目标进制数 base，并将每次的余数转换为字符后添加到结果字符串 result 中。余数小于10时，直接加上字符'0'；大于等于10时加上字符'A'。
如果输入的数字是负数，则在结果字符串的末尾加上负号。
最后反转字符串 result，以得到正确的进制表示，并返回该结果字符串。
（三）类 DecimalConverter
作用：这一类提供将其他进制的数（2到16进制）转换为十进制的功能。
函数 baseToDecimal(string number, int base)：
该函数接受一个字符串形式的进制数 number 和该数的进制 base。
初始化一个整数 result 为0，用于保存最终的十进制结果。
判断输入的字符串 number 是否以负号开始，若以负号开始，设置 isNegative 为 true，并去掉负号。
通过遍历字符串中的每个字符，将其转换为对应的十进制数值，并累加到 result 中。对于每个字符，如果是数字字符，则减去字符'0'；如果是字母字符，则先转换为大写，再减去字符'A'并加上10。
最后根据 isNegative 的值决定是否将 result 取反，然后返回 result。
（四）类 MetricConverter
作用：这一类提供长度、重量、体积等常用单位之间的转换功能。
函数：
meterToFeet(double meter)：将米转换为英尺，转换系数为3.28084。
feetToMeter(double feet)：将英尺转换为米。
kilometerToMile(double km)：将公里转换为英里，转换系数为0.621371。
mileToKilometer(double mile)：将英里转换为公里。
kilogramToPound(double kg)：将公斤转换为磅，转换系数为2.20462。
poundToKilogram(double pound)：将磅转换为公斤。
literToGallon(double liter)：将升转换为加仑，转换系数为0.264172。
gallonToLiter(double gallon)：将加仑转换为升。
（五）类 TimeZoneConverter
作用：这一类提供不同时区之间的时间转换功能。
函数 getOffset(string zone)：
该函数根据给定的时区名称 zone（中文），返回该时区相对于UTC的偏移量（小时）。如果输入的时区名称不在预设的支持列表中，返回-100以表示无效时区。
函数 convert(int hours, int minutes, int fromOffset, int toOffset)：
该函数接受两个时间（源时间和目标时间）以及它们所在的时区相对于UTC的偏移量 fromOffset 和 toOffset。
将源时间先转换为总的分钟数 totalMinutes。
调整时区差，得到目标时间的总分钟数。
处理跨日情况，确保时间在0-24小时之间。
最后将总分钟数转换为小时和分钟，并返回转换后的时间。
（六）逻辑实现：
设置控制台以支持中文显示。
进入一个无限循环，显示转换程序的主菜单，接受用户的选择。
根据用户的选择，分别调用相应的转换函数进行处理。
对于度量衡转换和时区转换，程序会进一步显示子菜单，让用户选择具体的转换类型。
对于输入的数值，程序会进行适当的验证，确保输入的值是有效的。
每次转换操作后，程序会等待用户按回车键继续，直到用户选择退出（选择0）为止。
