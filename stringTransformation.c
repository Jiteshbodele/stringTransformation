#include<stdio.h>
#include<stdbool.h>
#include<string.h>
 
 unsigned char prog[1000]="($first\n ($first 0x3\nX0<-#0x2\nX1<-#0X32\nX0*<-X1\n \n)\n)";
 //unsigned char prog[1000]=" ($first\n ($first 0xA \nRET\n CB W1<W2 B :first \nCB W1<W2 BL $first\nW0 >> #0x30 \n W0 *<- #0x33  \n BL $next\n BL $next\n BL $next\n BL $next \n:first\n mem X5 #0x70<-X6 \n)\n($firstx 0xA\n BL $next \n)\n($next 0X4 \n X28 <- mem X20 #-0X8 \n RET\n:name\n B :name\n BL $first \nX6 <- mem FP #0x10 \n)\n)\n\0";
int i=0;//to iterate throughout the given input program
int line=1;// to know the line no
int prtflg=0;// to know what to print in the final output 

char mainsbr[31];  // to store sbr from which the execution of program starts 
char temp[31]; //to store the string temporarily

enum instructis{load,store,sbranch, ldeclare,lbranch,move,compare,shift,arith,aritharr,ret};// insturctions can be of this types only 


struct instruction_data //struct to store an instuction
{
    enum instructis type;
    char destination[2][35];
    char operator[5];
    char operands[2][35];
};

struct sbr_data // structure for storing sbr data. At any index of its instance, we will get all the data under the sbr defination :sbrname,lables(called and declared),and all the insturctions
{
    char sdeclared[31];
    struct instruction_data instruction[50];
    int ins;
    char ldeclared [50][31];
    int ld;
    char lcalled [50][31];
    int lc;          
}sbrs[50];
int sd = 0; // iterator for sbr_data

char scalled[50][31];//for storing the called sbr
int sc = 0;// iterator for scalled 


bool  flag= false; //if the branch is declared it will be true otherwise flase


char tempstr[4][35];//array  to store the destination and source temporarilly
int ts=0;// iterator of tempstr
char tempop[4];//use to store operator temporary

char assembly[10000][35]; // array to  store the actual assembly instuctions which we have to print at last.
int as=0;//iterator of assembly


bool validate_firstline();//returns ture if first line is correct 
bool validate_program();//return true if program is correct
bool validate_sbrfirstline(); // returns true if first line of sbr is correct 
bool validate_sbr();// returns true if sbr is correct
bool validate_instruction();//returns true if instruction is correct
bool check_memoryinstruction();//checks insturctions which starts from memory
bool check_R32instruction();//checks insturctions which starts from R32
bool check_R64instruction();//checks insturctions which starts from R64
bool sbrdec_is_present ( struct sbr_data arr[], char str[],int itr);//checks if sbr name is similar or not and adds sbr name in sturcture accordingly
void sbrcal_is_present(char string[][31],char str[],int *itr);//checks if sbr name is similar or not and adds sbr name in sturcture accordingly
bool lbldec_is_present (struct sbr_data arr[],char str[]);//checks if lable name is similar or not and adds sbr name in sturcture accordingly
void lblcal_is_present (struct sbr_data arr[],char str[]);//checks if lable name is similar or not and adds sbr name in sturcture accordingly
bool check_calledlbl(struct sbr_data arr[]);//checks wheather the called called lable is present within corresponding sbr or not
bool check_calledsbr(struct sbr_data arr[],char calsbr[][31]);//checks wheather the called sbr is present in the program or not 
bool check_mainsbr ( struct sbr_data arr[] , char str[],int itr);//checks if main_sbr name is present in the declared function
void startend();//to make the block of start and end  in the array of assembly
void make_asm(int t);//converts instructions  to the assembly 

enum wordis {R32,R64,mem,lable,imm,number,immd8,arrow,oparrow,arth,nextline,open,close,name,sbrbranch,lablebranch,null,FP,RETURN,error,cmpop,cmpbr,shiftop}; //the words in the program can be of these types only


enum  wordis wordtype()//to check the type of nextword
{
int a=0; // for storing the char at its appropriate position in the string

switch (prog[i])
{


    case '\n': 
        while(prog[i]=='\n')
        {
            line++;
            i++;
            while(prog[i]==' '|| prog[i]=='\t')
            {
                i++;
            }
        }
        ts=0;         
        return(nextline);
        break;

        
    case ' ':
    case '\t':
        i++;
        wordtype();
        break;


    case '$':
        if ((prog[i+1]>='a'&& prog[i+1]<='z') || (prog[i+1]>='A'&& prog[i+1]<='Z'))
        {
            i++;
        
            while(((prog[i]>='a'&& prog[i]<='z') || (prog[i]>='A'&& prog[i]<='Z') || (prog[i]>='0'&& prog[i]<='9')) && a < 31)
            {
                temp[a]=prog[i];
                a++;
                i++;
            }
            temp[a]='\0';

            // logic to store the temp string in it's appropriate positoin
            if (mainsbr[0] == '\0')
            {
                strcpy(mainsbr,temp);
            }
            else if (flag == false)
            {
                    if (sbrdec_is_present(sbrs,temp,sd)== true)
                {
                    return(false);
                }
                
            }
            else if (flag == true)
            {
                strcpy(tempstr[ts],temp);
                ts++;
                sbrcal_is_present(scalled,temp,&sc);
            }
            

            return(name);
        }
        else 
        {
            return(error);
            
        }
        break;

        case ':':
        if((prog[i+1]>='a'&& prog[i+1]<='z') || (prog[i+1]>='A'&& prog[i+1]<='Z') )
        {
            i++;
            while(((prog[i]>='a'&& prog[i]<='z') || (prog[i+1]>='A'&& prog[i]<='Z') || (prog[i]>='0'&& prog[i]<='9')) && a<31)
            {
                temp[a]=prog[i];
                a++;
                i++;
            }
            temp[a]='\0';
            strcpy(tempstr[ts],temp);
            ts++;

            //logic to store the string into the appropriate position
            if (flag == false)
            {
                if (lbldec_is_present(sbrs,temp)==true)
                {
                    return(false);
                }
            }
            else if (flag == true)
            {
                lblcal_is_present(sbrs,temp);                  
            }
            return(lable);
        }
        else 
        {
            return(error);
        }
        break;


    case 'B':
        if(prog[i+1]=='L'&& prog[i+2]==' ')
        {   
            i+=3;
            flag = true;
            return(sbrbranch);
        }
        else if(prog[i+1]==' ')
        {
            i+=2;
            flag = true;
            return(lablebranch);
        }
        
            
    case '(':
        i++;
        return(open);
        break;


    case ')':
        i++;
        return(close);
        break;


    case '\0':
        i++;
        return(null);
        break;

        
    case '*':
        if (prog[i+1]=='<' && prog[i+2]=='-')
        {
            strcpy(tempop,"*<-");
            i=i+3;
            return(oparrow);
            break;
        }
        else if(prog[i+1]!='<')
        {
            strcpy(tempop,"*");
            i++;
            return(arth);
            break;
        }
        else 
        {
            return(error);
        }


    case '+':
        if (prog[i+1]=='<' && prog[i+2]=='-')
        {
            strcpy(tempop,"+<-");
            i=i+3;
            return(oparrow);
            break;
        }
        else if(prog[i+1]!='<')
        {
            strcpy(tempop,"+");
            i++;
            return(arth);
            break;
        }
        

    case '-':
        if (prog[i+1]=='<' && prog[i+2]=='-')
        {
            strcpy(tempop,"-<-");
            i=i+3;
            return(oparrow);
            break;
        }
        else if(prog[i+1]!='<')
        {
            strcpy(tempop,"-");
            i++;
            return(arth);
            break;
        }


    case '<':
        if (prog[i+1]=='-')
        {
            strcpy(tempop,"<-");
            i+=2;
            return(arrow);
            break;
        }
        else if(prog[i+1]=='<')
        {
            strcpy(tempop,"<<");
            i+=2;
            return(shiftop);
            break;
        }
        else if (prog[i+1]=='=')
        {
            strcpy(tempop,"<=");
            i+=2;
            return(cmpop);
            break;
        }
        else
        {
            strcpy(tempop,"<");
            i++;
            return(cmpop);
        }
    

    case '>':
        if(prog[i+1]=='>')
        {
            strcpy(tempop,">>");
            i+=2;
            return(shiftop);
        }
        else if (prog[i+1]=='=')
        {
            strcpy(tempop,">=");
            i+=2;
            return(cmpop);
            break;
        }
        else
        {
            strcpy(tempop,">");
            i++;
            return(cmpop);
            break;
        }


    case '=':
        if (prog[i+1]=='=')
        {
            strcpy(tempop,"==");
            i+=2;
            return(cmpop);
        }
        else if (prog[i+1]!='=')
        {
            return(error);
        }
    
    
    case '!':
        if (prog[i+1]=='=')
        {
            strcpy(tempop,"!=");
            i+=2;
            return(cmpop);
        }
        else if (prog[i+1]!='=')
        {
            return(error);
        }

    

    case 'W':
        if ((prog[i+1]>= '0' && prog[i+1]<='2') && (prog[i+2]>= '0' && prog[i+2]<='8') )
        {
            tempstr[ts][0]=prog[i];
            tempstr[ts][1]=prog[i+1];
            tempstr[ts][2]=prog[i+2];
            tempstr[ts][3]='\0';
            ts++;
            i+=3;
            return(R32);
        }
        else if (prog[i+1]>= '0' && prog[i+1]<='9')
        {
            tempstr[ts][0]=prog[i];
            tempstr[ts][1]=prog[i+1];
            tempstr[ts][2]='\0';
            ts++;
            i+=2;
            return(R32);
        }
        
        else
        {
            return(error);
        }


        case 'X':
        if ((prog[i+1]>= '0' && prog[i+1]<='2') && (prog[i+2]>= '0' && prog[i+2]<='8') )
        {
            tempstr[ts][0]=prog[i];
            tempstr[ts][1]=prog[i+1];
            tempstr[ts][2]=prog[i+2];
            tempstr[ts][3]='\0';
            ts++;
            i+=3;
            return(R64);
        }
        else if (prog[i+1]>= '0' && prog[i+1]<='9')
        {
            tempstr[ts][0]=prog[i];
            tempstr[ts][1]=prog[i+1];
            tempstr[ts][2]='\0';
            ts++;
            i+=2;
            return(R64);
        }
        
        else{
            return(error);
        }


    case '#':
        if((prog[i+1]=='-') && (prog[i+2]='0') && (prog[i+3]=='X'||'x') && ((prog[i+4]>='A'&& prog[i+4]<='F') || (prog[i+4]>='0'&& prog[i+4]<='9')))
        {
            int k=0;
            for (int j=i;j<i+5;j++)
            {
                tempstr[ts][k]=prog[j];
                k++; 
            }
            i+=5;
            while((prog[i]>='A'&& prog[i]<='F') || (prog[i]>='0'&& prog[i]<='9'))
            {
                tempstr[ts][k]=prog[i];
                k++;
                i++;
            }
            tempstr[ts][k]='\0';
            ts++;

            if (prog[i-1]=='0' || prog[i-1]=='8')
            {
                return(immd8);
            }
            else
            {
                return(imm);
            }
        }

        else if((prog[i+1]='0') && (prog[i+2]=='X' || prog[i+2]=='x') && ((prog[i+3]>='A'&& prog[i+3]<='F') || (prog[i+3]>='0'&& prog[i+3]<='9')))
        {
            int k=0;
            for (int j=i;j<i+4;j++)
            {
                tempstr[ts][k]=prog[j];
                k++; 
            }
            i+=4;
            while((prog[i]>='A'&& prog[i]<='F') || (prog[i]>='0'&& prog[i]<='9'))
            {
                tempstr[ts][k]=prog[i];
                k++;
                i++;
            }
            tempstr[ts][k]='\0';
            ts++;

            if (prog[i-1]=='0' || prog[i-1]=='8')
            {
                return(immd8);
            }
            else
            {
                return(imm);
            }
        }
        else{
            return(error);
        }
        break;


    case'0':
        if(prog[i+1]=='X' || prog[i+1]=='x' && (prog[i+2]>='A'&& prog[i+2]<='F') || (prog[i+2]>='0'&& prog[i+2]<='9'))
        {
            i+=3;
            while((prog[i]>='A'&& prog[i]<='F') || (prog[i]>='0'&& prog[i]<='9'))
            {
                i++;
            }
            return(number);

        }
        else
        {
            return(error);
        }
        break;


    case 'm':
        if (prog[i+1]== 'e' && prog[i+2]=='m')
        {
            i+=3;
            return(mem);
        }
        else
        {
            return(error);
        }
        break;

    
    case 'R':
        if (prog[i+1]=='E' && prog[i+2]=='T' )
        {
            i+=3;
            return(RETURN);
        }
        else
        {
            return(error);
        }
        break;


    case 'F':
    {
        if (prog[i+1]=='P')
        {
            strcpy(tempstr[ts],"FP");
            ts++;
            i+=2;
            return(FP);
        }
        else
        {
            return(error);
        }
        break;
    }

    case 'C':
    {
        if (prog[i+1]=='B'&& prog[i+2]==' ')
        {
            i+=3;
            return(cmpbr);
        }
        else
        {
            return(error);
        }
    }

        

    default:
        return(error);
        break;        

}
}


bool validate_program()//return true if program is correct
{ 
  switch (wordtype())
  {
    case open:
        
        return(validate_firstline());
        break;
    case nextline:
        validate_program();
        break;
    default:
        return(false);
        break;
  }

}
   

bool validate_firstline()//returns ture if first line is correct 
{
    if (wordtype()==name && wordtype()==nextline)
    {
        startend();
        return(validate_sbr());
    }
    else
    {
        return(false);
    }
}


bool validate_sbr()// returns true if sbr is correct
{
    if (wordtype()==open)
    {
        return(validate_sbrfirstline());
    }
    else
    {
        return(false);
    }

}


bool validate_sbrfirstline() // returns true if first line of sbr is correct 
{
    sbrs[sd].lc = 0;//whenever the new sbr is declared the iterators of the array which stores lable_declarations , lable_calls and insturctions_info  will initialize from 0;
    sbrs[sd].ld = 0;
    sbrs[sd].ins = 0;
    if (wordtype()==name && wordtype()==number && wordtype()==nextline )
    {
        char dunderscore[35]="__";
        strcpy(assembly[as++],strcat(strcat(dunderscore,sbrs[sd].sdeclared),":\n"));
        strcpy(assembly[as++],"\nBL start\n") ;
        return(validate_instruction());
    }
    else
    {
        return(false);
    }
}


bool validate_instruction()//returns true if instruction is correct   
{
    switch (wordtype())
    {
        case close:       //if ( comes  after checking instruction then call validatie_sbr  //else if ) comes after checking then return ture;  //else call validate_instruction
            if (check_calledlbl(sbrs)==false)
            {
                return(false);
            }
            for (int t=0; t<sbrs[sd].ins;t++)
            {
                make_asm(t);
            }
            strcpy(assembly[as++],"B end\n\n") ;

            sd++; // if current sbr is closing the iterator of my structure which stores the info of array will increase as it stores info of one sbr at one index
            if(wordtype()==nextline)
            {
                switch (wordtype())
                {
                case open:
                    return(validate_sbrfirstline());
                    break;


                case close:
                    if ((wordtype()==null) || (wordtype()==nextline && wordtype()==null))
                    {
                        if (check_mainsbr(sbrs,mainsbr,sd)==false)
                        {
                            return(false);
                        }
                        else if(check_calledsbr(sbrs,scalled)==false)
                        {
                            return(false);
                        }
                        
                        return(true);
                    }
                    else
                    {
                        return(false);
                    }
                    break;
                
                default:
                    return(false);
                    break;
                }
            }
            else
            {
                return(false);
            }
            break;


        case lable: // if lable comes check next insturction
            if (wordtype()==nextline)
            {
                sbrs[sd].instruction[sbrs[sd].ins].type=ldeclare;
                strcpy(sbrs[sd].instruction[sbrs[sd].ins].destination[0],tempstr[0]);
                sbrs[sd].ins++;
                return(validate_instruction());
            }
            else 
            {
                return(false);
            }
            break;


        case lablebranch://if lablebranch comes check if lable name is next to it and then check for next insturction
            if (wordtype()==lable && wordtype() == nextline)
            {   
                sbrs[sd].instruction[sbrs[sd].ins].type=lbranch;
                strcpy(sbrs[sd].instruction[sbrs[sd].ins].destination[0],tempstr[0]);
                sbrs[sd].ins++;       
                return(validate_instruction());
            }
            else
            {
                return(false);
            }
            break;


        case sbrbranch://if sbrbranch comes check if sbr name is next to it and then check for next insturction
            if (wordtype()==name && wordtype() == nextline)
            {      
                sbrs[sd].instruction[sbrs[sd].ins].type=sbranch;
                strcpy(sbrs[sd].instruction[sbrs[sd].ins].destination[0],tempstr[0]);
                sbrs[sd].ins++;     
                return(validate_instruction());
            }
            else
            {
                return(false);
            }
            break;
        

        case cmpbr:
            switch (wordtype())
            {
                case R32:
                    ;
                    int d1=wordtype();
                    int d2=wordtype();
                    if ((d1==cmpop) && (d2==R32||d2==imm||d2==immd8))
                    {
                        switch(wordtype())
                        {
                            case lablebranch:
                                if (wordtype()==lable && wordtype()==nextline)
                                {
                                    sbrs[sd].instruction[sbrs[sd].ins].type=compare;
                                    strcpy(sbrs[sd].instruction[sbrs[sd].ins].operands[0],tempstr[0]);
                                    strcpy(sbrs[sd].instruction[sbrs[sd].ins].operands[1],tempstr[1]);
                                    strcpy(sbrs[sd].instruction[sbrs[sd].ins].destination[0],tempstr[2]);
                                    strcpy(sbrs[sd].instruction[sbrs[sd].ins].destination[1],"lable");
                                    strcpy(sbrs[sd].instruction[sbrs[sd].ins].operator,tempop);
                                    sbrs[sd].ins++;
                                    return(validate_instruction());
                                }
                                else
                                {
                                    return(false);
                                }
                            case sbrbranch:
                               
                                if (wordtype()==name && wordtype()==nextline)
                                {
                                    sbrs[sd].instruction[sbrs[sd].ins].type=compare;
                                    strcpy(sbrs[sd].instruction[sbrs[sd].ins].operands[0],tempstr[0]);
                                    strcpy(sbrs[sd].instruction[sbrs[sd].ins].operands[1],tempstr[1]);
                                    strcpy(sbrs[sd].instruction[sbrs[sd].ins].destination[0],tempstr[2]);
                                    strcpy(sbrs[sd].instruction[sbrs[sd].ins].destination[1],"sbr");
                                    strcpy(sbrs[sd].instruction[sbrs[sd].ins].operator,tempop);
                                    sbrs[sd].ins++;
                                    return(validate_instruction());
                                }
                                else
                                {
                                    return(false);
                                }
                        }
                    }
                    else 
                    {
                        return(false);
                    }

                case R64:
                    ;
                     d1=wordtype();
                     d2=wordtype();
                    if ((d1==cmpop) && (d2==R64||d2==imm||d2==immd8))
                    {
                        switch(wordtype())
                        {
                            case lablebranch:
                                if (wordtype()== lable && wordtype()==nextline)
                                {
                                    sbrs[sd].instruction[sbrs[sd].ins].type=compare;
                                    strcpy(sbrs[sd].instruction[sbrs[sd].ins].operands[0],tempstr[0]);
                                    strcpy(sbrs[sd].instruction[sbrs[sd].ins].operands[1],tempstr[1]);
                                    strcpy(sbrs[sd].instruction[sbrs[sd].ins].destination[0],tempstr[2]);
                                    strcpy(sbrs[sd].instruction[sbrs[sd].ins].operator,tempop);
                                    sbrs[sd].ins++;
                                    return(validate_instruction());
                                }
                                else
                                {
                                    return(false);
                                }
                            case sbrbranch:
                                if (wordtype()==name && wordtype()==nextline)
                                {
                                    sbrs[sd].instruction[sbrs[sd].ins].type=compare;
                                    strcpy(sbrs[sd].instruction[sbrs[sd].ins].operands[0],tempstr[0]);
                                    strcpy(sbrs[sd].instruction[sbrs[sd].ins].operands[1],tempstr[1]);
                                    strcpy(sbrs[sd].instruction[sbrs[sd].ins].destination[0],tempstr[2]);
                                    strcpy(sbrs[sd].instruction[sbrs[sd].ins].operator,tempop);
                                    sbrs[sd].ins++;
                                    return(validate_instruction());
                                }
                                else
                                {
                                    return(false);
                                }
                        }
                    }
                    else 
                    {
                        return(false);
                    }
            }
            

        case mem:
            return(check_memoryinstruction());
            break;

        case R32:
            return(check_R32instruction());
            break;
        
        case R64:
            return(check_R64instruction());
            break;
        
       

        case RETURN:
            if (wordtype()==nextline)
            {
                sbrs[sd].instruction[sbrs[sd].ins].type=ret;
                strcpy(sbrs[sd].instruction[sbrs[sd].ins].destination[0],"RET");
                sbrs[sd].ins++;
                return(validate_instruction());
            }
            else
            {
                return(false);
            } 
        default:
            return(false);
            break;
    }
}


bool check_memoryinstruction()//checks insturctions which starts from memory
{
    enum wordis d =wordtype();
    if ((d==FP ||d== R64) && (wordtype()==immd8) && (wordtype()==arrow) && (wordtype()==R64)&& (wordtype()==nextline) )
    {
        sbrs[sd].instruction[sbrs[sd].ins].type=store;
        strcpy(sbrs[sd].instruction[sbrs[sd].ins].destination[0],tempstr[0]);
        strcpy(sbrs[sd].instruction[sbrs[sd].ins].destination[1],tempstr[1]);
        strcpy(sbrs[sd].instruction[sbrs[sd].ins].operands[0],tempstr[2]);
        strcpy(sbrs[sd].instruction[sbrs[sd].ins].operator,tempop);
        sbrs[sd].ins++;
        return(validate_instruction());
    }

    else
    {
        return(false);
    }
}


bool check_R32instruction()//checks insturctions which starts from R32
{
    switch (wordtype())
    {
        case arrow:
            switch(wordtype())
            {
                case R32:
                    switch (wordtype())
                    {
                        case nextline:
                            sbrs[sd].instruction[sbrs[sd].ins].type=move;
                            strcpy(sbrs[sd].instruction[sbrs[sd].ins].destination[0],tempstr[0]);
                            strcpy(sbrs[sd].instruction[sbrs[sd].ins].operands[0],tempstr[1]);
                            strcpy(sbrs[sd].instruction[sbrs[sd].ins].operator,tempop);
                            sbrs[sd].ins++;    
                            return(validate_instruction());
                            break;

                        case immd8:                
                        case imm:
                            if (wordtype()==nextline)
                            {
                                sbrs[sd].instruction[sbrs[sd].ins].type=arith;
                                strcpy(sbrs[sd].instruction[sbrs[sd].ins].destination[0],tempstr[0]);
                                strcpy(sbrs[sd].instruction[sbrs[sd].ins].operands[0],tempstr[1]);
                                strcpy(sbrs[sd].instruction[sbrs[sd].ins].operands[1],tempstr[2]);
                                strcpy(sbrs[sd].instruction[sbrs[sd].ins].operator,"+");
                                sbrs[sd].ins++; 
                                return(validate_instruction());
                            }
                            else 
                            {
                                return(false);
                            }
                            
                        
                        case arth:
                            if (wordtype()==R32 &&  wordtype()==nextline)
                            {
                                sbrs[sd].instruction[sbrs[sd].ins].type=arith;
                                strcpy(sbrs[sd].instruction[sbrs[sd].ins].destination[0],tempstr[0]);
                                strcpy(sbrs[sd].instruction[sbrs[sd].ins].operands[0],tempstr[1]);
                                strcpy(sbrs[sd].instruction[sbrs[sd].ins].operands[1],tempstr[2]);
                                strcpy(sbrs[sd].instruction[sbrs[sd].ins].operator,tempop);
                                sbrs[sd].ins++; 
                                return(validate_instruction());
                            }
                            else
                            {
                                return(false);
                            }
                        default:
                            return(false);
                            break;
                    }


                case immd8:
                case imm:
                    if (wordtype()==nextline)
                    {
                        sbrs[sd].instruction[sbrs[sd].ins].type=move;
                        strcpy(sbrs[sd].instruction[sbrs[sd].ins].destination[0],tempstr[0]);
                        strcpy(sbrs[sd].instruction[sbrs[sd].ins].operands[0],tempstr[1]);
                        strcpy(sbrs[sd].instruction[sbrs[sd].ins].operator,tempop);
                        sbrs[sd].ins++; 
                        return(validate_instruction());
                    }
                    else 
                    {
                        return(false);
                    }
                
                default:
                    return(false);
                    break;
            }
        

        case oparrow:
            ;
            enum wordis d =wordtype();
            if ((d==R32||d==imm||d==immd8) && (wordtype()==nextline))
            {
                sbrs[sd].instruction[sbrs[sd].ins].type=aritharr;
                strcpy(sbrs[sd].instruction[sbrs[sd].ins].destination[0],tempstr[0]);
                strcpy(sbrs[sd].instruction[sbrs[sd].ins].operands[0],tempstr[1]);
                strcpy(sbrs[sd].instruction[sbrs[sd].ins].operator,tempop);
                sbrs[sd].ins++; 
                return(validate_instruction());
            }
            else
            {
                return(false);
            }
            break;

        case shiftop:
            d=wordtype();
            if ((d==immd8||d==imm||d==R32)&& (wordtype()==nextline))
            {
                sbrs[sd].instruction[sbrs[sd].ins].type=shift;
                strcpy(sbrs[sd].instruction[sbrs[sd].ins].destination[0],tempstr[0]);
                strcpy(sbrs[sd].instruction[sbrs[sd].ins].operands[0],tempstr[1]);
                strcpy(sbrs[sd].instruction[sbrs[sd].ins].operator,tempop);
                sbrs[sd].ins++; 
                return(validate_instruction());
            }
            else
            {
                return(false);
            }

        default:
            return(false);
            break;
    }
}


bool check_R64instruction()//checks insturctions which starts from R64
{
    switch (wordtype())
    {
        case arrow:
            switch(wordtype())
            {
                case R64:
                
                    switch (wordtype())
                    {
                        case nextline:
                            sbrs[sd].instruction[sbrs[sd].ins].type=move;
                            strcpy(sbrs[sd].instruction[sbrs[sd].ins].destination[0],tempstr[0]);
                            strcpy(sbrs[sd].instruction[sbrs[sd].ins].operands[0],tempstr[1]);
                            strcpy(sbrs[sd].instruction[sbrs[sd].ins].operator,tempop);
                            sbrs[sd].ins++;
                            return(validate_instruction());
                            break;
                        case immd8:
                        case imm:
                            if (wordtype()==nextline)
                            {
                                sbrs[sd].instruction[sbrs[sd].ins].type=arith;
                                strcpy(sbrs[sd].instruction[sbrs[sd].ins].destination[0],tempstr[0]);
                                strcpy(sbrs[sd].instruction[sbrs[sd].ins].operands[0],tempstr[1]);
                                strcpy(sbrs[sd].instruction[sbrs[sd].ins].operands[1],tempstr[2]);
                                strcpy(sbrs[sd].instruction[sbrs[sd].ins].operator,"+");
                                sbrs[sd].ins++; 
                                return(validate_instruction());
                            }
                            else 
                            {
                                return(false);
                            }
                            
                        
                        case arth:
                            if (wordtype()==R64 &&  wordtype()==nextline)
                            {
                                sbrs[sd].instruction[sbrs[sd].ins].type=arith;
                                strcpy(sbrs[sd].instruction[sbrs[sd].ins].destination[0],tempstr[0]);
                                strcpy(sbrs[sd].instruction[sbrs[sd].ins].operands[0],tempstr[1]);
                                strcpy(sbrs[sd].instruction[sbrs[sd].ins].operands[1],tempstr[2]);
                                strcpy(sbrs[sd].instruction[sbrs[sd].ins].operator,tempop);
                                sbrs[sd].ins++; 
                                return(validate_instruction());
                            }
                            else
                            {
                                return(false);
                            }
                        default:
                            return(false);
                            break;
                    }
                    
                case mem:
                
                    ;
                    enum wordis d =wordtype();
                    if ((d==FP || d==R64) && (wordtype()==immd8) && (wordtype()==nextline))
                    {
                        sbrs[sd].instruction[sbrs[sd].ins].type=load;
                        strcpy(sbrs[sd].instruction[sbrs[sd].ins].destination[0],tempstr[0]);
                        strcpy(sbrs[sd].instruction[sbrs[sd].ins].operands[0],tempstr[1]);
                        strcpy(sbrs[sd].instruction[sbrs[sd].ins].operands[1],tempstr[2]);
                        strcpy(sbrs[sd].instruction[sbrs[sd].ins].operator,tempop);
                        sbrs[sd].ins++;
                        return(validate_instruction());
                    }
                    else
                    {
                        return(false);
                    }
                case immd8:
                case imm:
                    if (wordtype()==nextline)
                    {
                        sbrs[sd].instruction[sbrs[sd].ins].type=move;
                        strcpy(sbrs[sd].instruction[sbrs[sd].ins].destination[0],tempstr[0]);
                        strcpy(sbrs[sd].instruction[sbrs[sd].ins].operands[0],tempstr[1]);
                        strcpy(sbrs[sd].instruction[sbrs[sd].ins].operator,tempop);
                        sbrs[sd].ins++; 
                        return(validate_instruction());
                    }
                    else
                    {
                        return(false);
                    }
                    
                default:
                    return(false);
                    break;

        }

        case oparrow:
            ;
            enum wordis d =wordtype();
            if ((d==R64||d==imm||d==immd8) && (wordtype()==nextline))
            {
                sbrs[sd].instruction[sbrs[sd].ins].type=aritharr;
                strcpy(sbrs[sd].instruction[sbrs[sd].ins].destination[0],tempstr[0]);
                strcpy(sbrs[sd].instruction[sbrs[sd].ins].operands[0],tempstr[1]);
                strcpy(sbrs[sd].instruction[sbrs[sd].ins].operator,tempop);
                sbrs[sd].ins++; 
                return(validate_instruction());
            }
            else
            {
                return(false);
            }
            break;

        case shiftop:
            ;
            d=wordtype();
            if ((d==immd8||d==imm||d==R64)&& (wordtype()==nextline))
            {
                sbrs[sd].instruction[sbrs[sd].ins].type=shift;
                strcpy(sbrs[sd].instruction[sbrs[sd].ins].destination[0],tempstr[0]);
                strcpy(sbrs[sd].instruction[sbrs[sd].ins].operands[0],tempstr[1]);
                strcpy(sbrs[sd].instruction[sbrs[sd].ins].operator,tempop);
                sbrs[sd].ins++; 
                return(validate_instruction());
            }
            else
            {
                return(false);
            }
        
        
        default:
            return(false);
            break;
    }
}


bool sbrdec_is_present ( struct sbr_data arr[] , char str[],int itr)//checks if sbr name is similar or not and adds sbr name in sturcture accordingly
 {
    for (int j=0; j<itr ; ++j)
    {
        if (strcmp(arr[j].sdeclared,str) == 0)
        {
            prtflg=1;
            printf("declaration of two sbrs having same names:-\"%s\"\n ",str);
            return(true);
        }
        
    }
    strcpy(sbrs[itr].sdeclared,str);
    return(false);
 }


void sbrcal_is_present(char string[][31],char str[],int * itr)//checks if sbr name is similar or not and adds sbr name in sturcture accordingly
 {
    flag = false;//ensuring that the branch instruction is over
    for (int j=0; j<*itr ; ++j)
    {
        if (strcmp(scalled[j],str) == 0)
        {
            return;
        }
        
    }
    strcpy(scalled[*itr],str);
    (*itr)++;
    //sc=itr;
    return;
 }


bool lbldec_is_present (struct sbr_data arr[],char str[])//checks if lable name is similar or not and adds sbr name in sturcture accordingly
{
    for (int j=0;j<sbrs[sd].ld;++j)
    {
        if (strcmp(sbrs[sd].ldeclared[j],str)==0)
        {
            prtflg=1;
            printf("within the scope of sbr \"%s\" there is the declaration of two lables having name :-\"%s\"\n",sbrs[sd].sdeclared,str);
            return(true);
        }
    }

    strcpy(sbrs[sd].ldeclared[sbrs[sd].ld],str);
    sbrs[sd].ld++;
    return(false);   
}


void lblcal_is_present (struct sbr_data arr[],char str[])//checks if lable name is similar or not and adds sbr name in sturcture accordingly
{
    flag= false; //ensuring that the branch insturction is over
    for (int j=0;j<sbrs[sd].lc;++j)
    {
        if (strcmp(sbrs[sd].lcalled[j],str)==0)
        {
            return;
        }
    }

    strcpy(sbrs[sd].lcalled[sbrs[sd].lc],str);
    sbrs[sd].lc++;
    return;

    
}


bool check_calledlbl(struct sbr_data arr[])//checks wheather the called called lable is present within corresponding sbr or not
{
    
    for (int j=0; j<sbrs[sd].lc ; j++)
    {
        int r=0;
        for (int k=0; k<sbrs[sd].ld; k++)
        {
            if (strcmp(sbrs[sd].lcalled[j],sbrs[sd].ldeclared[k])==0)
            {
                r=1;
            }
        }
        if (r==0)
        {
            prtflg=1;
            printf("defination of called lable:- \"%s\" doesn't exist  in the sbr:- \"%s\"\n",sbrs[sd].lcalled[j],sbrs[sd].sdeclared);
            return(false);
        }
    }
    return(true);

}


bool check_calledsbr(struct sbr_data arr[],char calsbr[][31])//checks wheather the called sbr is present in the program or not 
{
    for (int j=0; j<sc ; j++)
    {
        int r=0;
        for (int k=0; k<sd; k++)
        {
            if (strcmp(calsbr[j],sbrs[k].sdeclared)==0)
            {
                r=1;
            }
        }
        if (r==0)
        {
            prtflg=1;
            printf("defination of called sbr:- \"%s\" doesn't exist in the program",calsbr[j]);
            return(false);
        }
    }
    return(true);



}


bool check_mainsbr ( struct sbr_data arr[] , char str[],int itr)//checks if main_sbr name is present in the declared function
 {
    for (int j=0; j<itr ; ++j)
    {
        if (strcmp(arr[j].sdeclared,str) == 0)
        {
            return(true);
        }
        
    }
    prtflg=1;
    printf("defination of main_sbr :-\"%s\" is missing in the program ",mainsbr);
    return(false);
 }


void startend()//to make the block of start and end  in the array of assembly
{
    strcpy(assembly[as++],".section .text\n");
    strcpy(assembly[as++],".global _start\n\n");
    strcpy(assembly[as++],"_start:\n");
    strcpy(assembly[as++],"\tmov X0, #1\n");
    strcpy(assembly[as++],"\tBL __");
    strcpy(assembly[as++], mainsbr);
    strcpy(assembly[as++],"\n ");
    strcpy(assembly[as++],"\tmov w8, #93\n");
    strcpy(assembly[as++],"\tsvc #0\n");
    strcpy(assembly[as++],"\tret\n\n");
    strcpy(assembly[as++],"start:\n");
    strcpy(assembly[as++],"STR FP, [SP, #-8]!\n");
    strcpy(assembly[as++],"STR LR, [SP, #-8]!\n");
    strcpy(assembly[as++],"MOV FP, SP\n");
    strcpy(assembly[as++],"RET\n\n");
    strcpy(assembly[as++],"end:\n");
    strcpy(assembly[as++],"MOV SP, FP\n");
    strcpy(assembly[as++],"LDR LR, [SP], #8\n");
    strcpy(assembly[as++],"LDR FP, [SP], #8\n");
    strcpy(assembly[as++],"RET\n\n");   
}


void make_asm(int t)//converts instructions  to the assembly 
{
    switch (sbrs[sd].instruction[t].type)
    {
        case load:
            strcpy(assembly[as++],"LDR ");
            strcpy(assembly[as++],strcat(sbrs[sd].instruction[t].destination[0],", ["));
            strcpy(assembly[as++],strcat(sbrs[sd].instruction[t].operands[0],", "));
            strcpy(assembly[as++],strcat(sbrs[sd].instruction[t].operands[1],"]\n")); 
            break;

        case store:
            strcpy(assembly[as++],"STR [");
            strcpy(assembly[as++],strcat(sbrs[sd].instruction[t].destination[0],", "));
            strcpy(assembly[as++],strcat(sbrs[sd].instruction[t].destination[1],"], "));
            strcpy(assembly[as++],strcat(sbrs[sd].instruction[t].operands[0],"\n")); 
            break;

        case sbranch:
            strcpy(assembly[as++],"BL __");
            strcpy(assembly[as++],strcat(sbrs[sd].instruction[t].destination[0],"\n"));
            break;

        case ldeclare:
            ;
            char underscore[35]="_";
            strcpy(assembly[as++],strcat(strcat(underscore,sbrs[sd].instruction[t].destination[0]),":\n"));
            break;

        case lbranch:
            strcpy(assembly[as++],"B _");
            strcpy(assembly[as++],strcat(sbrs[sd].instruction[t].destination[0],"\n"));
            break; 

        case move:
            strcpy(assembly[as++],"MOV ");
            strcpy(assembly[as++],strcat(sbrs[sd].instruction[t].destination[0],", "));
            strcpy(assembly[as++],strcat(sbrs[sd].instruction[t].operands[0],"\n"));

            break;

        case compare:
            strcpy(assembly[as++],"CMP ");
            strcpy(assembly[as++],strcat(sbrs[sd].instruction[t].operands[0],", "));
            strcpy(assembly[as++],strcat(sbrs[sd].instruction[t].operands[1],"\n"));
            char cmpunderscore[35]="_";
            char cmpdunderscore[35]="__";
            if (strcmp(sbrs[sd].instruction[t].operator,"<")==0)
            {
                strcpy(assembly[as++],"BLT ");
                if (strcmp(sbrs[sd].instruction[t].destination[1],"lable")==0)
                {
                    strcpy(assembly[as++],strcat(strcat(cmpunderscore,sbrs[sd].instruction[t].destination[0]),"\n"));
                }
                else if (strcmp(sbrs[sd].instruction[t].destination[1],"sbr")==0)
                {
                    strcpy(assembly[as++],strcat(strcat(cmpdunderscore,sbrs[sd].instruction[t].destination[0]),"\n"));
                }
            }
            else if (strcmp(sbrs[sd].instruction[t].operator,"<=")==0)
            {
                strcpy(assembly[as++],"BLE ");
                if (strcmp(sbrs[sd].instruction[t].destination[1],"lable")==0)
                {
                    strcpy(assembly[as++],strcat(strcat(cmpunderscore,sbrs[sd].instruction[t].destination[0]),"\n"));
                }
                else if (strcmp(sbrs[sd].instruction[t].destination[1],"sbr")==0)
                {
                    strcpy(assembly[as++],strcat(strcat(cmpdunderscore,sbrs[sd].instruction[t].destination[0]),"\n"));
                }

            }
            else if (strcmp(sbrs[sd].instruction[t].operator,">")==0)
            {
                strcpy(assembly[as++],"BGT ");
                if (strcmp(sbrs[sd].instruction[t].destination[1],"lable")==0)
                {
                    strcpy(assembly[as++],strcat(strcat(cmpunderscore,sbrs[sd].instruction[t].destination[0]),"\n"));
                }
                else if (strcmp(sbrs[sd].instruction[t].destination[1],"sbr")==0)
                {
                    strcpy(assembly[as++],strcat(strcat(cmpdunderscore,sbrs[sd].instruction[t].destination[0]),"\n"));
                }

            }
            else if (strcmp(sbrs[sd].instruction[t].operator,">=")==0)
            {
                strcpy(assembly[as++],"BGE ");
                if (strcmp(sbrs[sd].instruction[t].destination[1],"lable")==0)
                {
                    strcpy(assembly[as++],strcat(strcat(cmpunderscore,sbrs[sd].instruction[t].destination[0]),"\n"));
                }
                else if (strcmp(sbrs[sd].instruction[t].destination[1],"sbr")==0)
                {
                    strcpy(assembly[as++],strcat(strcat(cmpdunderscore,sbrs[sd].instruction[t].destination[0]),"\n"));
                }

            }
            else if (strcmp(sbrs[sd].instruction[t].operator,"==")==0)
            {
                strcpy(assembly[as++],"BEQ ");
                if (strcmp(sbrs[sd].instruction[t].destination[1],"lable")==0)
                {
                    strcpy(assembly[as++],strcat(strcat(cmpunderscore,sbrs[sd].instruction[t].destination[0]),"\n"));
                }
                else if (strcmp(sbrs[sd].instruction[t].destination[1],"sbr")==0)
                {
                    strcpy(assembly[as++],strcat(strcat(cmpdunderscore,sbrs[sd].instruction[t].destination[0]),"\n"));
                }

            }
            else if (strcmp(sbrs[sd].instruction[t].operator,"!=")==0)
            {
                strcpy(assembly[as++],"BNE ");
                if (strcmp(sbrs[sd].instruction[t].destination[1],"lable")==0)
                {
                    strcpy(assembly[as++],strcat(strcat(cmpunderscore,sbrs[sd].instruction[t].destination[0]),"\n"));
                }
                else if (strcmp(sbrs[sd].instruction[t].destination[1],"sbr")==0)
                {
                    strcpy(assembly[as++],strcat(strcat(cmpdunderscore,sbrs[sd].instruction[t].destination[0]),"\n"));
                }

            }
             
            break;

        case shift:
            if (strcmp(sbrs[sd].instruction[t].operator,"<<")==0)
            {
                strcpy(assembly[as++],"LSL ");
                strcpy(assembly[as++],strcat(sbrs[sd].instruction[t].destination[0],", "));
                strcpy(assembly[as++],sbrs[sd].instruction[t].destination[0]);
                strcpy(assembly[as++],strcat(sbrs[sd].instruction[t].operands[0],"\n"));
            }
            else if (strcmp(sbrs[sd].instruction[t].operator,"<<")!=0)
            {
                strcpy(assembly[as++],"LSR ");
                strcpy(assembly[as++],strcat(sbrs[sd].instruction[t].destination[0],", "));
                strcpy(assembly[as++],sbrs[sd].instruction[t].destination[0]);
                strcpy(assembly[as++],strcat(sbrs[sd].instruction[t].operands[0],"\n"));
            }
                break;

        case arith:
            if (strcmp(sbrs[sd].instruction[t].operator,"+")==0)
            {
                strcpy(assembly[as++],"ADD ");
                strcpy(assembly[as++],strcat(sbrs[sd].instruction[t].destination[0],", "));
                strcpy(assembly[as++],strcat(sbrs[sd].instruction[t].operands[0],", "));
                strcpy(assembly[as++],strcat(sbrs[sd].instruction[t].operands[1],"\n"));
            }
            else if (strcmp(sbrs[sd].instruction[t].operator,"*")==0)
            {
                strcpy(assembly[as++],"MUL ");
                strcpy(assembly[as++],strcat(sbrs[sd].instruction[t].destination[0],", "));
                strcpy(assembly[as++],strcat(sbrs[sd].instruction[t].operands[0],", "));
                strcpy(assembly[as++],strcat(sbrs[sd].instruction[t].operands[1],"\n"));
            }
            else if (strcmp(sbrs[sd].instruction[t].operator,"-")==0)
            {
                strcpy(assembly[as++],"SUB ");
                strcpy(assembly[as++],strcat(sbrs[sd].instruction[t].destination[0],", "));
                strcpy(assembly[as++],strcat(sbrs[sd].instruction[t].operands[0],", "));
                strcpy(assembly[as++],strcat(sbrs[sd].instruction[t].operands[1],"\n"));
            }
            break;

        case aritharr:
            if (strcmp(sbrs[sd].instruction[t].operator,"+<-")==0)
            {
                strcpy(assembly[as++],"ADD ");
                strcpy(assembly[as++],strcat(sbrs[sd].instruction[t].destination[0],", "));
                strcpy(assembly[as++],sbrs[sd].instruction[t].destination[0]);
                strcpy(assembly[as++],strcat(sbrs[sd].instruction[t].operands[0],"\n"));
            }
            else if (strcmp(sbrs[sd].instruction[t].operator,"*<-")==0)
            {
                strcpy(assembly[as++],"MUL ");
                strcpy(assembly[as++],strcat(sbrs[sd].instruction[t].destination[0],", "));
                strcpy(assembly[as++],sbrs[sd].instruction[t].destination[0]);
                strcpy(assembly[as++],strcat(sbrs[sd].instruction[t].operands[0],"\n"));
            }
            else if (strcmp(sbrs[sd].instruction[t].operator,"-<-")==0)
            {
                strcpy(assembly[as++],"SUB ");
                strcpy(assembly[as++],strcat(sbrs[sd].instruction[t].destination[0],", "));
                strcpy(assembly[as++],sbrs[sd].instruction[t].destination[0]);
                strcpy(assembly[as++],strcat(sbrs[sd].instruction[t].operands[0],"\n"));
            }
            break;

        case ret:
            strcpy(assembly[as++],strcat(sbrs[sd].instruction[t].destination[0],"\n"));
            break;
        
        default:
            break;
    }

}

void main ()
{    FILE *fp;
	fp = fopen("test.s","w");
  //  bool d=validate_program();
    if (validate_program()==true)
    {
       // printf("programe is verified successfully !! \nAll the lables and sbrs are  defined correctly\n");
       // printf("\nresult :- \n");
        int i=0;
        while(assembly[i][0]!='\0')
        {

    fputs(assembly[i],fp);
            printf("%s",assembly[i]);
            i++;
        }
    }
    else if (validate_program()==false && prtflg==1 )
    {
        printf("error at line no.:- %d\n",line);
    }
    else if (validate_program()==false && prtflg==0 )
    {
        printf(" syntax error at line no.:- %d\n",line);
    }

    fclose(fp);
 }
