#include <iostream>
#include <unordered_set>
#include <string.h>

//������ģ��
class BlackListModule {
public:
    BlackListModule() {
        // Ԥ��������绰����
        blacklist = {"1234567890", "0987654321", "1111111111"};
    }
    // Ԥ��Ľӿ� ���ǲ���Ҫ���ڲ�����
    bool inBlackList(const char* number) {
        return blacklist.find(number) != blacklist.end();
    }

private:
    std::unordered_set<std::string> blacklist;
};

//����׮����
bool testInBlackList(const char* number) {
    //ʹ��Ԥ��ĺ���������
    if (strcmp(number, "1234567890") == 0 || strcmp(number, "0987654321") == 0 || strcmp(number, "1111111111") == 0) {
        return true;
    } else {
        return false;
    }
}

int main() {
    BlackListModule module;
    //�����õĺ���
    const char* number = "12345678901";
    //ʹ�ò���׮����Ԥ��Ľӿ�
    if (testInBlackList(number)) {
        std::cout << "The number is in the black list." << std::endl;
    } else {
        std::cout << "The number is not in the black list." << std::endl;
    }
    system("pause");
    return 0;
}