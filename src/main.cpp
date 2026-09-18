#include "FileManager.h"
#include "ReportManager.h"
#include "Ui.h"
#include <iostream>

int main(){
    try{
        FileManager fm;
        ReportManager rm(&fm);
        ui::session(&fm, &rm);
    }catch(const std::exception& e){
        std::cerr << "Fatal error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}