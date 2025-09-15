#include<stdio.h>
#include<stdbool.h>

unsigned char prog[1000]=" \n   \t  \n($first\n  \t \n  \n\t ($first 0xA \n W0<-W2\n X0<-#0x5 \n mem X5 #0x70<-X6  \n)\n($next 0X4 \n X28 <- mem X20 #-0X8 \n RET \n:name\n W0<-W5+W4\nB :name\n W0 <-W1\nBL \t $first \nX6 <- mem FP #0x10 \n)\n)\n\0";
int i=0;
int line=1;


  enum wordis {R32,R64,mem,lable,imm,number,immd8,arrow,oparrow,arth,nextline,open,close,name,sbrbranch,lablebranch,null,FP,RETURN,error}; //the words in the program can be of these types only
 
 
  enum  wordis wordtype()//to check the type of nextword
  {
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
                while((prog[i]>='a'&& prog[i]<='z') || (prog[i]>='A'&& prog[i]<='Z') || (prog[i]>='0'&& prog[i]<='9'))
                {
                    i++;
                }
                
                return(name);
            }
            else 
            {
                return(error);
                
            }
            break;


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
                i=i+3;
                return(oparrow);
                break;
            }
            else if(prog[i]!='<')
            {
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
                i=i+3;
                return(oparrow);
                break;
            }
            else 
            {
                i++;
                return(arth);
                break;
            }
            

        case '-':
            if (prog[i+1]=='<' && prog[i+2]=='-')
            {
                i=i+3;
                return(oparrow);
                break;
            }
            else 
            {
                i++;
                return(arth);
                break;
            }


        case '<':
            if (prog[i+1]=='-')
            {
                i+=2;
                return(arrow);
                break;
            }
            else 
            {
                i+=2;
                return(arth);
                break;
            }
        

        case '>':
            if(prog[i+1]=='>')
            {
                i+=2;
                return(arth);
            }
            else {
                return(error);
            }


        case ':':
            if((prog[i+1]>='a'&& prog[i+1]<='z') || (prog[i+1]>='A'&& prog[i+1]<='Z') )
            {
                i++;
                while((prog[i]>='a'&& prog[i]<='z') || (prog[i+1]>='A'&& prog[i]<='Z') || (prog[i]>='0'&& prog[i]<='9'))
                {
                    i++;
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
            {   i+=3;
                return(sbrbranch);
            }
            else if(prog[i+1]==' ')
            {
                i+=2;
                return(lablebranch);
            }

        case 'W':
            if ((prog[i+1]>= '0' && prog[i+1]<='2') && (prog[i+2]>= '0' && prog[i+2]<='8') )
            {
                i+=3;
                return(R32);
            }
            else if (prog[i+1]>= '0' && prog[i+1]<='9')
            {
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
                i+=3;
                return(R64);
            }
            else if (prog[i+1]>= '0' && prog[i+1]<='9')
            {
                i+=2;
                return(R64);
            }
            
            else{
                return(error);
            }


        case '#':
            if((prog[i+1]=='-') && (prog[i+2]='0') && (prog[i+3]=='X' || prog[i+3]=='x') && ((prog[i+4]>='A'&& prog[i+4]<='F') || (prog[i+4]>='0'&& prog[i+4]<='9')))
            {
                i+=5;
                while((prog[i]>='A'&& prog[i]<='F') || (prog[i]>='0'&& prog[i]<='9'))
                {
                    i++;
                }
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
                i+=4;
                while((prog[i]>='A'&& prog[i]<='F') || (prog[i]>='0'&& prog[i]<='9'))
                {
                    i++;
                }
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
                i+=2;
                return(FP);
            }
            else
            {
                return(error);
            }
            break;
        }


            

        default:
            return(error);
            break;        

    }
}


bool validate_firstline();//returns ture if first line is correct 
bool validate_program();//return true if program is correct
bool validate_sbrfirstline(); // returns true if first line of sbr is correct 
bool validate_sbr();// returns true if sbr is correct
bool validate_instruction();//returns true if instruction is correct
bool check_memoryinstruction();//checks insturctions which starts from memory
bool check_R32instruction();//checks insturctions which starts from R32
bool check_R64instruction();//checks insturctions which starts from R64




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
    if (wordtype()==name && wordtype()==number && wordtype()==nextline )
    {
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
            if(wordtype()==nextline)
            {
                switch (wordtype())
                {
                case open:
                    return(validate_sbrfirstline());
                    break;


                case close:
                    if ((wordtype()==nextline && wordtype()==null)||(wordtype()==null))
                    {
                        return(true);
                    }
                    else
                    {
                        return(false);
                    }
                    break;
                
                default:
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
                return(validate_instruction());
            }
            else
            {
                return(false);
            }
            break;

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
                            return(validate_instruction());
                            break;

                    case immd8:                
                    case imm:
                        if (wordtype()==nextline)
                        {
                            return(validate_instruction());
                        }
                        else 
                        {
                            return(false);
                        }
                        
                    
                    case arth:
                        if (wordtype()==R32 &&  wordtype()==nextline)
                        {
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
            return(validate_instruction());
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
                            return(validate_instruction());
                            break;
                        case immd8:
                        case imm:
                            if (wordtype()==nextline)
                            {
                                return(validate_instruction());
                            }
                            else 
                            {
                                return(false);
                            }
                            
                        
                        case arth:
                            if (wordtype()==R64 &&  wordtype()==nextline)
                            {
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
                return(validate_instruction());
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


void main ()
{   
    
    bool d=validate_program();
    if (d==true)
    {
        printf("program is valid\n");
    }
    else
    {
        printf("error at line no.:- %d\n",line);
    }
}