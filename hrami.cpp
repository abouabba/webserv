# include <iostream>


void printa(std::string s) {
    std::cout << "a"<< s << "\n";
}
void printb(std::string s) {
    std::cout <<"b"<< s << "\n";
}
void printc(std::string s) {
    std::cout << "c"<<s << "\n";
}

int main() {


    void (*ptr[])(std::string) = {printa, printb, printc};

    ptr[1]("dfkjnfo");
}