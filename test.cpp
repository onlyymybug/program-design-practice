#include <iostream>
#include <unordered_set>
#include <string.h>

//黑名单模块
class BlackListModule {
public:
    BlackListModule() {
        // 预设黑名单电话号码
        blacklist = {"1234567890", "0987654321", "1111111111"};
    }
    // 预设的接口 但是不需要用在测试中
    bool inBlackList(const char* number) {
        return blacklist.find(number) != blacklist.end();
    }

private:
    std::unordered_set<std::string> blacklist;
};

//测试桩函数
bool testInBlackList(const char* number) {
    //使用预设的黑名单号码
    if (strcmp(number, "1234567890") == 0 || strcmp(number, "0987654321") == 0 || strcmp(number, "1111111111") == 0) {
        return true;
    } else {
        return false;
    }
}

int main() {
    BlackListModule module;
    //测试用的号码
    const char* number = "12345678901";
    //使用测试桩而非预设的接口
    if (testInBlackList(number)) {
        std::cout << "The number is in the black list." << std::endl;
    } else {
        std::cout << "The number is not in the black list." << std::endl;
    }
    system("pause");
    return 0;
}