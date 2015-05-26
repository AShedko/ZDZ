#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>
#include <fstream>
#include <unistd.h>   //_getch
#include <termios.h>  //_getch
#include <stdio.h>      /* printf */
#include <time.h>       /* clock_t, clock, CLOCKS_PER_SEC */
#include <sstream>

using namespace std;

char getch(){
    char buf=0;
    struct termios old={0};
    fflush(stdout);
    if(tcgetattr(0, &old)<0)
        perror("tcsetattr()");
    old.c_lflag&=~ICANON;
    old.c_lflag&=~ECHO;
    old.c_cc[VMIN]=1;
    old.c_cc[VTIME]=0;
    if(tcsetattr(0, TCSANOW, &old)<0)
        perror("tcsetattr ICANON");
    if(read(0,&buf,1)<0)
        perror("read()");
    old.c_lflag|=ICANON;
    old.c_lflag|=ECHO;
    if(tcsetattr(0, TCSADRAIN, &old)<0)
        perror ("tcsetattr ~ICANON");
    printf("%c\n",buf);
    return buf;
 }

// Sorting subroutine;
void sort(vector <int>* vec,int l,int r){
    int i,j,swp,pivot;
    if (r>0){                                 //checking for empty vector
        do{
            i=l;
            j=r;
            pivot=(*vec)[(rand())%(r-l)+l];   //randomly choosing the pivot element from elements of the array
            do{
                while ((*vec)[i]<pivot) i++;  //finding the element smaller then pivot
                while ((*vec)[j]>pivot) j--;  //finding the element greater then pivot
                if (i<=j){
                    swp=(*vec)[i];
                    (*vec)[i]=(*vec)[j];      //swapping
                    (*vec)[j]=swp;
                    i++;
                    j--;
                }
            }while (i<=j);
            if ((j-l)<(r-i)){                 // finding the minimal subvector
                if (l<j)
                    sort(vec,l,j);            // qsorting the smaller subvector
                l=i;
            } else{
                if (i<r)
                    sort(vec,i,r);            // qsorting the smaller subvector
                r=j;
            }
        } while (l!=r);
    }
}

void qsort(vector <int>* vec){
    sort(vec,0,vec->size()-1);
}

int main()
{
    string fileName,inpStr,temp;
    srand(time(NULL));
    char a;
    int inp=0;
    clock_t time;
    vector <int> vec;
    while (1){
        cout << "Integer number sequence quicksorting enviroment.\n"
                "-------------------------------------------------------------------------\n"
                "Input action: 1- add element to vector, 2- clear vector, 3- qsort vector\n"
                " 4- show vector,5- erase element on nth position, 6- store vector to file\n"
                " 7- append vector from file, 8- exit program.\n";
        cin >> a;
        switch (a){
        case '1':{                           //Pseudo-menu (self-explainatory)
            cout << "Input element\n";
            cin >> inpStr;
            inp=strtol(inpStr.c_str(),0,10);
            stringstream ss;
            ss<<inp;
            ss>>temp;
            if (temp!=inpStr){
                cout<<" Input error!";
                cout<<"\nPress enter to continue\n";
                getch();
            }
            else {
                vec.push_back(inp);
                getch();
            }
            break;
        }
        case '2':{
            vec.clear();
            break;
        }
        case '3':{
            time = clock();
            qsort(&vec);
            time = clock()-time;
            printf ("Sorting time: %d clicks (%f seconds).\n",time,((float)time)/CLOCKS_PER_SEC);
            cout<<"\nPress enter to continue\n";
            getch();
            break;
        }
        case '4':{
            for (int i=0;i<(int)vec.size();i++){
                cout<<vec[i]<<" ";
                if (i%10==0 && i)
                    cout<<"\n";
            }
            cout<<"\nPress enter to continue\n";
            getch();
            break;
        }
        case '5':{
            cout<<"Input the position of the element to erase\n";
            cin>>inp;
            if (inp>0&&inp<=(int)vec.size())
                (vec.erase(vec.begin()+inp-1));
            else
                cout<<"Error, no such element\n";
        }
        case '6':{
            ofstream outFile;
            cout <<"Input name of the output file\n";
            cin >> fileName;
            outFile.open(fileName.c_str(),ios_base::trunc); // Clears the file before writing
            if (!outFile)
                cout<<"IO Error!!!\n";
            else{
                for (int i=0;i<(int)vec.size();i++){
                    outFile<<vec[i]<<" ";
                    if (i%10==0 && i)
                        outFile<<"\n";
                }
                outFile<<"\n";
            }
            outFile.close();
            break;
        }
        case '7':{
            ifstream inputFile;

            cout <<"Input name of the file \n";
            cin >> fileName;
            inputFile.open((fileName).c_str());   //open method has a cstring argument
            if (!inputFile)
                cout <<"IO error!\n";
            else{
                while (inputFile >> inpStr){
                    inp=strtol(inpStr.c_str(),0,10);
                    stringstream ss;
                    ss<<inp;
                    ss>>temp;
                    if (temp!=inpStr){
                        cout<<" Input error!";
                        cout<<"\nPress enter to continue\n";
                        getch();
                    }
                    else {
                        vec.push_back(inp);
                        getch();
                    }
                }
            inputFile.close();
            break;
        }
        }
        case '8':{
            return 0;
            break;
        }
        default:{
            cout << "Input error, Please try again.\n";
            break;
        }
        }
        if (system("CLS")) system("clear");
    }
}

