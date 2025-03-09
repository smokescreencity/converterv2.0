#include <iostream>
#include <string>
#include <algorithm>
#include <locale>
#include <windows.h>
#include <fcntl.h>
#include <io.h>
#include <iomanip>  // 添加这个头文件以支持 setprecision
#include <limits>   // 添加这个头文件以支持 numeric_limits
using namespace std;

// 十进制转换为其他进制
class BaseConverter {
public:
    string decimalToBase(int decimal, int base) {
        if (decimal == 0) return "0";

        string result;
        // 判断给定的十进制数是否为负数
        bool isNegative = decimal < 0;

        decimal = abs(decimal);//取绝对值

        while (decimal > 0) {
            int remainder = decimal % base;
                // 如果余数小于10，将余数转换为ASCII码对应的数字字符（0-9）
            if (remainder < 10)
                result.push_back(remainder + '0');
                // 如果余数大于等于10，将余数转换为ASCII码对应的字母字符（A-F）
            else
                result.push_back(remainder - 10 + 'A');
                 // 将原数除以进制基数，继续处理下一位数字
            decimal /= base;
        }

        if (isNegative)
            result.push_back('-');

        reverse(result.begin(), result.end());
        return result;
    }
};

// 其他进制转换为十进制
class DecimalConverter {
public:
    int baseToDecimal(string number, int base) {
        int result = 0;
        bool isNegative = false;

        if (number[0] == '-') {
            isNegative = true;
            number = number.substr(1);
        }

        for (char digit : number) {
            result *= base;
            if (isdigit(digit))
                result += digit - '0';
            else if (isalpha(digit))
                result += toupper(digit) - 'A' + 10;
        }

        return isNegative ? -result : result;
    }
};

// 时区转换类
class TimeZoneConverter {
public:
    // 将一个时区的时间转换为另一个时区的时间
    pair<int, int> convertTimeZone(int hours, int minutes, int fromOffset, int toOffset) {
        if (hours < 0 || hours >= 24) {
            throw invalid_argument("小时必须在0-23之间");
        }
        if (minutes < 0 || minutes >= 60) {
            throw invalid_argument("分钟必须在0-59之间");
        }
        if (fromOffset < -12 || fromOffset > 14 || toOffset < -12 || toOffset > 14) {
            throw invalid_argument("时区偏移量必须在-12到+14之间");
        }

        // 转换为UTC时间（以分钟为单位）
        int totalMinutes = hours * 60 + minutes;
        totalMinutes -= fromOffset * 60;  // 先转换到UTC时间
        
        // 从UTC转换到目标时区
        totalMinutes += toOffset * 60;
        
        // 处理跨日情况
        while (totalMinutes < 0) {
            totalMinutes += 24 * 60;
        }
        totalMinutes %= (24 * 60);
        
        return make_pair(totalMinutes / 60, totalMinutes % 60);
    }
    
    // 获取常见时区的UTC偏移量
    int getOffsetByZone(const wstring& zoneName) {
        // 转换为大写以实现不区分大小写的比较
        wstring upperZone = zoneName;
        transform(upperZone.begin(), upperZone.end(), upperZone.begin(), ::toupper);
        
        if (upperZone == L"北京" || upperZone == L"BEIJING" || upperZone == L"CST") return 8;
        if (upperZone == L"东京" || upperZone == L"TOKYO" || upperZone == L"JST") return 9;
        if (upperZone == L"伦敦" || upperZone == L"LONDON" || upperZone == L"GMT") return 0;
        if (upperZone == L"纽约" || upperZone == L"NEW YORK" || upperZone == L"EST") return -5;
        if (upperZone == L"洛杉矶" || upperZone == L"LOS ANGELES" || upperZone == L"PST") return -8;
        if (upperZone == L"悉尼" || upperZone == L"SYDNEY" || upperZone == L"AEDT") return 11;
        if (upperZone == L"莫斯科" || upperZone == L"MOSCOW" || upperZone == L"MSK") return 3;
        if (upperZone == L"迪拜" || upperZone == L"DUBAI" || upperZone == L"GST") return 4;
        
        throw invalid_argument("不支持的时区名称");
    }
    
    // 格式化时间输出
    wstring formatTime(int hours, int minutes) {
        wstring result;
        
        // 添加小时，确保两位数
        if (hours < 10) result += L"0";
        result += to_wstring(hours) + L":";
        
        // 添加分钟，确保两位数
        if (minutes < 10) result += L"0";
        result += to_wstring(minutes);
        
        return result;
    }
};

// 辅助函数：解析时间字符串
pair<int, int> parseTime(const wstring& timeStr) {
    try {
        size_t colonPos = timeStr.find(L':');
        if (colonPos == wstring::npos) {
            throw invalid_argument("时间格式错误，请使用HH:MM格式");
        }
        
        int hours = stoi(timeStr.substr(0, colonPos));
        int minutes = stoi(timeStr.substr(colonPos + 1));
        
        if (hours < 0 || hours >= 24) {
            throw invalid_argument("小时必须在0-23之间");
        }
        if (minutes < 0 || minutes >= 60) {
            throw invalid_argument("分钟必须在0-59之间");
        }
        
        return {hours, minutes};
    } catch (...) {
        throw invalid_argument("时间格式错误，请使用HH:MM格式（例如：08:30）");
    }
}

// 度量衡转换类
class MetricConverter {
public:
    // 长度转换
    double meterToFeet(double meter) { return meter * 3.28084; }
    double feetToMeter(double feet) { return feet / 3.28084; }
    
    double kilometerToMile(double km) { return km * 0.621371; }
    double mileToKilometer(double mile) { return mile / 0.621371; }
    
    double centimeterToInch(double cm) { return cm * 0.393701; }
    double inchToCentimeter(double inch) { return inch / 0.393701; }
    
    // 重量转换
    double kilogramToPound(double kg) { return kg * 2.20462; }
    double poundToKilogram(double pound) { return pound / 2.20462; }
    
    double gramToOunce(double gram) { return gram * 0.035274; }
    double ounceToGram(double ounce) { return ounce / 0.035274; }
    
    // 体积转换
    double literToGallon(double liter) { return liter * 0.264172; }
    double gallonToLiter(double gallon) { return gallon / 0.264172; }
    
    double milliliterToFlOz(double ml) { return ml * 0.033814; }
    double flOzToMilliliter(double flOz) { return flOz / 0.033814; }
    
    // 面积转换
    double squareMeterToSquareFeet(double sqm) { return sqm * 10.7639; }
    double squareFeetToSquareMeter(double sqft) { return sqft / 10.7639; }
    
    double hectareToAcre(double hectare) { return hectare * 2.47105; }
    double acreToHectare(double acre) { return acre / 2.47105; }
};

int main() {
    // 设置控制台UTF-8支持
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    
    // 设置标准输出为UTF-8模式
    _setmode(_fileno(stdout), _O_U8TEXT);
    _setmode(_fileno(stdin), _O_U8TEXT);
    
    // 设置本地化
    setlocale(LC_ALL, "zh_CN.UTF-8");
    
    int choice;
    wstring number;
    int decimal;

    while (true) {
        wcout << L"--------------------------------" << endl;
        wcout << L"\n你好，欢迎使用转换程序\n";
        wcout << L"1. 十进制转二进制\n";
        wcout << L"2. 十进制转八进制\n";
        wcout << L"3. 十进制转十六进制\n";
        wcout << L"4. 二进制转十进制\n";
        wcout << L"5. 八进制转十进制\n";
        wcout << L"6. 十六进制转十进制\n";
        wcout << L"7. 时区转换\n";
        wcout << L"8. 度量衡转换\n";
        wcout << L"0. 退出\n";
        wcout << L"请选择操作 (0-8): ";
        wcout << L"--------------------------------" << endl;

        wcin >> choice;

        if (choice == 0) break;

        switch (choice) {
        case 1: {
            wcout << L"请输入十进制数: ";
            wcin >> decimal;
            BaseConverter converter;
            wcout << L"二进制结果: " << converter.decimalToBase(decimal, 2).c_str() << endl;
            break;
        }
        case 2: {
            wcout << L"请输入十进制数: ";
            wcin >> decimal;
            BaseConverter converter;
            wcout << L"八进制结果: " << converter.decimalToBase(decimal, 8).c_str() << endl;
            break;
        }
        case 3: {
            wcout << L"请输入十进制数: ";
            wcin >> decimal;
            BaseConverter converter;
            wcout << L"十六进制结果: " << converter.decimalToBase(decimal, 16).c_str() << endl;
            break;
        }
        case 4: {
            wcout << L"请输入二进制数: ";
            wcin >> number;
            DecimalConverter converter;
            wcout << L"十进制结果: " << converter.baseToDecimal(string(number.begin(), number.end()), 2) << endl;
            break;
        }
        case 5: {
            wcout << L"请输入八进制数: ";
            wcin >> number;
            DecimalConverter converter;
            wcout << L"十进制结果: " << converter.baseToDecimal(string(number.begin(), number.end()), 8) << endl;
            break;
        }
        case 6: {
            wcout << L"请输入十六进制数: ";
            wcin >> number;
            DecimalConverter converter;
            wcout << L"十进制结果: " << converter.baseToDecimal(string(number.begin(), number.end()), 16) << endl;
            break;
        }
        case 7: {
            try {
                wstring timeStr, fromZone, toZone;
                
                wcout << L"请输入时间 (格式 HH:MM): ";
                wcin >> timeStr;
                
                auto [hours, minutes] = parseTime(timeStr);
                
                wcout << L"请输入源时区 (如: 北京, 东京, 伦敦, 纽约, 洛杉矶): ";
                wcin.ignore();  // 清除之前的换行符
                getline(wcin, fromZone);
                
                wcout << L"请输入目标时区 (如: 北京, 东京, 伦敦, 纽约, 洛杉矶): ";
                getline(wcin, toZone);
                
                TimeZoneConverter converter;
                int fromOffset = converter.getOffsetByZone(fromZone);
                int toOffset = converter.getOffsetByZone(toZone);
                
                pair<int, int> result = converter.convertTimeZone(hours, minutes, fromOffset, toOffset);
                
                wcout << fromZone << L" " << converter.formatTime(hours, minutes)
                     << L" 对应 " << toZone << L" " 
                     << converter.formatTime(result.first, result.second) << endl;
            }
            catch (const exception& e) {
                wcout << L"错误: " << e.what() << endl;
            }
            break;
        }
        case 8: {
            try {
                wcout << L"\n度量衡转换类型：\n";
                wcout << L"1. 长度转换\n";
                wcout << L"2. 重量转换\n";
                wcout << L"3. 体积转换\n";
                wcout << L"4. 面积转换\n";
                wcout << L"请选择转换类型 (1-4): ";
                
                int metricChoice;
                wcin >> metricChoice;
                
                MetricConverter converter;
                double value;
                
                switch(metricChoice) {
                    case 1: {
                        wcout << L"\n长度转换选项：\n";
                        wcout << L"1. 米 → 英尺\n";
                        wcout << L"2. 英尺 → 米\n";
                        wcout << L"3. 公里 → 英里\n";
                        wcout << L"4. 英里 → 公里\n";
                        wcout << L"5. 厘米 → 英寸\n";
                        wcout << L"6. 英寸 → 厘米\n";
                        wcout << L"请选择转换方式 (1-6): ";
                        
                        int choice;
                        wcin >> choice;
                        wcout << L"请输入数值: ";
                        wcin >> value;
                        
                        wcout << fixed << setprecision(3);
                        switch(choice) {
                            case 1:
                                wcout << value << L" 米 = " << converter.meterToFeet(value) << L" 英尺" << endl;
                                break;
                            case 2:
                                wcout << value << L" 英尺 = " << converter.feetToMeter(value) << L" 米" << endl;
                                break;
                            case 3:
                                wcout << value << L" 公里 = " << converter.kilometerToMile(value) << L" 英里" << endl;
                                break;
                            case 4:
                                wcout << value << L" 英里 = " << converter.mileToKilometer(value) << L" 公里" << endl;
                                break;
                            case 5:
                                wcout << value << L" 厘米 = " << converter.centimeterToInch(value) << L" 英寸" << endl;
                                break;
                            case 6:
                                wcout << value << L" 英寸 = " << converter.inchToCentimeter(value) << L" 厘米" << endl;
                                break;
                            default:
                                wcout << L"无效的选择" << endl;
                        }
                        break;
                    }
                    case 2: {
                        wcout << L"\n重量转换选项：\n";
                        wcout << L"1. 公斤 → 磅\n";
                        wcout << L"2. 磅 → 公斤\n";
                        wcout << L"3. 克 → 盎司\n";
                        wcout << L"4. 盎司 → 克\n";
                        wcout << L"请选择转换方式 (1-4): ";
                        
                        int choice;
                        wcin >> choice;
                        wcout << L"请输入数值: ";
                        wcin >> value;
                        
                        wcout << fixed << setprecision(3);
                        switch(choice) {
                            case 1:
                                wcout << value << L" 公斤 = " << converter.kilogramToPound(value) << L" 磅" << endl;
                                break;
                            case 2:
                                wcout << value << L" 磅 = " << converter.poundToKilogram(value) << L" 公斤" << endl;
                                break;
                            case 3:
                                wcout << value << L" 克 = " << converter.gramToOunce(value) << L" 盎司" << endl;
                                break;
                            case 4:
                                wcout << value << L" 盎司 = " << converter.ounceToGram(value) << L" 克" << endl;
                                break;
                            default:
                                wcout << L"无效的选择" << endl;
                        }
                        break;
                    }
                    case 3: {
                        wcout << L"\n体积转换选项：\n";
                        wcout << L"1. 升 → 加仑\n";
                        wcout << L"2. 加仑 → 升\n";
                        wcout << L"3. 毫升 → 液量盎司\n";
                        wcout << L"4. 液量盎司 → 毫升\n";
                        wcout << L"请选择转换方式 (1-4): ";
                        
                        int choice;
                        wcin >> choice;
                        wcout << L"请输入数值: ";
                        wcin >> value;
                        
                        wcout << fixed << setprecision(3);
                        switch(choice) {
                            case 1:
                                wcout << value << L" 升 = " << converter.literToGallon(value) << L" 加仑" << endl;
                                break;
                            case 2:
                                wcout << value << L" 加仑 = " << converter.gallonToLiter(value) << L" 升" << endl;
                                break;
                            case 3:
                                wcout << value << L" 毫升 = " << converter.milliliterToFlOz(value) << L" 液量盎司" << endl;
                                break;
                            case 4:
                                wcout << value << L" 液量盎司 = " << converter.flOzToMilliliter(value) << L" 毫升" << endl;
                                break;
                            default:
                                wcout << L"无效的选择" << endl;
                        }
                        break;
                    }
                    case 4: {
                        wcout << L"\n面积转换选项：\n";
                        wcout << L"1. 平方米 → 平方英尺\n";
                        wcout << L"2. 平方英尺 → 平方米\n";
                        wcout << L"3. 公顷 → 英亩\n";
                        wcout << L"4. 英亩 → 公顷\n";
                        wcout << L"请选择转换方式 (1-4): ";
                        
                        int choice;
                        wcin >> choice;
                        wcout << L"请输入数值: ";
                        wcin >> value;
                        
                        wcout << fixed << setprecision(3);
                        switch(choice) {
                            case 1:
                                wcout << value << L" 平方米 = " << converter.squareMeterToSquareFeet(value) << L" 平方英尺" << endl;
                                break;
                            case 2:
                                wcout << value << L" 平方英尺 = " << converter.squareFeetToSquareMeter(value) << L" 平方米" << endl;
                                break;
                            case 3:
                                wcout << value << L" 公顷 = " << converter.hectareToAcre(value) << L" 英亩" << endl;
                                break;
                            case 4:
                                wcout << value << L" 英亩 = " << converter.acreToHectare(value) << L" 公顷" << endl;
                                break;
                            default:
                                wcout << L"无效的选择" << endl;
                        }
                        break;
                    }
                    default:
                        wcout << L"无效的选择" << endl;
                }
                
                wcout << L"\n按回车键继续...";
                wcin.ignore(numeric_limits<streamsize>::max(), L'\n');
                wcin.get();
            }
            catch (const exception& e) {
                wcout << L"错误: " << e.what() << endl;
            }
            break;
        }
        
        default:
            wcout << L"无效的选择\n";
        }
    }

    return 0;
}