#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <string>
#include <string.h>

using namespace std;

int main(int argc, char * argv[]){
  if ( argc == 4 ){
    chrono::time_point<chrono::system_clock> start, end;
    time_t start_t, end_t;

    int size = 1024 * 1024 * 10;

    cout << "Timing your compressing" << endl; 

    string cmd_tmp;
    cmd_tmp = "./compress ";
    cmd_tmp += argv[1];
    cmd_tmp += " ";
    cmd_tmp += argv[2];
    cmd_tmp += '\0';

    char cmd[cmd_tmp.size()];
    for ( int i = 0; i < cmd_tmp.size(); i++ ){
      cmd[i] = cmd_tmp[i];
    }

    cout << cmd << endl;

    start = chrono::system_clock::now();
    start_t = chrono::system_clock::to_time_t(start);
    cout << "Compressng operation starts at " << ctime(&start_t);
    system( cmd  );
    end = chrono::system_clock::now();
    chrono::duration<double> seconds = end-start;
    end_t = chrono::system_clock::to_time_t(end);
    cout << "Finished running at " << ctime(&end_t);
    cout << "Elapsed time of compressing\t" << seconds.count() << endl;
    
    cout << "Timing your uncompressing" << endl;
    
    cmd_tmp = "./uncompress ";
    cmd_tmp += argv[2];
    cmd_tmp += " ";
    cmd_tmp += argv[3];
    cmd_tmp += '\0';
   
    char cmd1[cmd_tmp.size()];
    for ( int i = 0; i < cmd_tmp.size(); i++ ){
      cmd1[i] = cmd_tmp[i];
    }

    cout << cmd1 << endl;

    start = chrono::system_clock::now();
    start_t = chrono::system_clock::to_time_t(start);
    cout << "Uncompressng operation starts at " << ctime(&start_t);
    system( cmd1 );
    end = chrono::system_clock::now();
    seconds = end-start;
    end_t = chrono::system_clock::to_time_t(end);
    cout << "Finished running at " << ctime(&end_t);
    cout << "Elapsed time of uncompressing\t" << seconds.count() << endl;
  }
  else{
    cout << "Please specify an input, an temporary zip file and an output file!" << endl;
  }
  return 0;
}
