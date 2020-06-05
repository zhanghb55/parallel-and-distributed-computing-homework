//
//  Created by zhb on 2020/5/1.
//  Copyright © 2020 zhb. All rights reserved.
//
#include "analyse.h"

int main(int argc, const char * argv[]) {
    if(argc != 2){
        cout << "Please input the LLVM IR file name!" << endl;
        abort();
    }
    string file = read_file(argv[1]);
    divide(file);
    Non_reentrant_fun.push_back("printf");
    Non_reentrant_fun.push_back("scanf");
    Non_reentrant_fun.push_back("malloc");
    Non_reentrant_fun.push_back("free");

    bool res = false;
    for(int i = 0;i < all_function.size();i++){
        module test(all_function[i]);
        bool ret = test.detect_and_print();
        if(ret)res = true;
    }
    if(!res){
        cout << "Non reentrant function not detected." << endl;
    }
}

