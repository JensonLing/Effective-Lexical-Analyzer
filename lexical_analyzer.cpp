# include <iostream>
# include <map>
# include <queue>
# include <string>
# include <fstream>

//# define EOF 0

using namespace std;

map<string,int> tokens;//包含所有可识别的符号及保留字，可添加
map<int,string> token_type; //数字所代表的符号类型，固定
map<int,int> token_num;//用以统计各类型记号的出现次数
map<string, int> operators;
queue<char> token_queue;
string err_info;
int line = 0;

bool header_flag = false;
bool crossing_flag = false;
bool annotation = false;

void load_code(string path)
{

}
void read_op_table(string path)
{
    fstream t(path,ios::in|ios::out);
    char temp[1024];
    string out;
    while(t.getline(temp,1024))
    {
        int type = atoi(temp);
        //cout<<"get:"<<type<<endl;
        t.getline(temp,1024);
        string s = temp;
        operators[s] = type;
         //cout<<s.length();

        //out.append("\0");
    }
    t.close();
}

int judge_type(char c)
{
    char temp_c[2];
    temp_c[0] =  c;

    if(annotation)
        return 10;

    if(c == ' ')
        return 0;
    else if(c == '_' || (c >= 'a' && c <= 'z') || (c>='A' && c <= 'Z'))
        return 1;
    else if(c>='0' && c<='9')
        return 2;
    else if(operators[temp_c])
        return operators[temp_c];
    else if(c == EOF)
        return -1;
    else if(c == '\n')
        return -4;
    else
        return -2;
}

void analyze(char* s)
{
    int i = 0;
    int state = judge_type(s[i]);
    bool over = false;
    while(!over)
    {
        //state = judge_type(s[i]);
        //cout<<"in_while";
        switch(state)
        {
            case 0://空格，无需输出
                cout<<"case0:";
                i++;
                state = judge_type(s[i]);
                //cout<<"next:"<<state;
                break;
            case 1:
                cout<<"case1:";
                token_queue.push(s[i]);
                //cout<<"pushed:"<<s[i]<<endl;
                i++;
                if(judge_type(s[i]) == 1 || judge_type(s[i]) == 2)
                {
                    state = 1;
                }
                else
                {
                    while(!token_queue.empty())
                    {
                        //cout<<"pop:";
                        cout<<token_queue.front();
                        token_queue.pop();
                    }
                    cout<<" ";
                    state = judge_type(s[i]);
                    cout<<"exit_state:"<<state<<" ";
                }
                break;
            case 2:
                //cout<<"2->"<<"pushed:"<<s[i]<<'~';
                cout<<" state2:" ;
                token_queue.push(s[i]);
                //cout<<"front:"<<token_queue.front()<<'~';
                i++;
                if(judge_type(s[i]) == 2)
                    state = 2;
                else if(s[i] == '.')
                {
                    token_queue.push(s[i]);
                    //cout<<"pushed:"<<s[i]<<'~';
                    //cout<<"front:"<<token_queue.front()<<'~';
                    i++;
                    state = 3;
                }
                else if(s[i] == 'e')
                {
                    token_queue.push(s[i]);
                    i++;
                    state = 5;
                }
                else if(judge_type(s[i]) == 1)
                {
                    state = -6;
                }
                else
                {
                    state = judge_type(s[i]);
                    while(!token_queue.empty())
                    {
                        //cout<<"pop:";
                        cout<<token_queue.front();
                        token_queue.pop();
                    }
                    cout<<" ";
                }
                
                break;
            case 3:
                //cout<<"3->";
                cout<<" state3:" ;
                if(judge_type(s[i]) == 2)
                {
                    token_queue.push(s[i]);
                    i++;
                    state = 4;
                }
                else if(s[i] == '.')
                {
                    i++;
                    state = 3;
                    //ERR
                }
                else if(judge_type(s[i]) == 1)
                {
                    //token_queue.push('0');
                    state = -6;/*
                    while(!token_queue.empty())
                    {
                        //cout<<"pop:";
                        cout<<token_queue.front();
                        token_queue.pop();
                    }
                    cout<<" ";*/
                }
                else
                {
                    token_queue.push('0');
                    //state = judge_type(s[i]);
                    state = -5;
                    while(!token_queue.empty())
                    {
                        //cout<<"pop:";
                        cout<<token_queue.front();
                        token_queue.pop();
                    }
                    cout<<" ";
                }
                break;
            case 4:
                cout<<" state4:" ;
                if(judge_type(s[i]) == 2)
                {
                    token_queue.push(s[i]);
                    i++;
                    state = 4;//问题：读到换行符就不读了，没来得及pop(已解决 )
                }
                else if(s[i] == 'e')
                {
                    token_queue.push(s[i]);
                    i++;
                    state = 5;
                }
                else if(s[i] == '.')
                {
                    i++;
                    state = 4;
                    //ERR
                }
                else if(judge_type(s[i]) == 1)
                {
                    state = -6;/*
                    while(!token_queue.empty())
                    {
                        //cout<<"pop:";
                        cout<<token_queue.front();
                        token_queue.pop();
                    }
                    cout<<" ";*/
                }
                else
                {
                    //state = judge_type(s[i]);
                    state = judge_type(s[i]);
                    while(!token_queue.empty())
                    {
                        //cout<<"pop:";
                        cout<<token_queue.front();
                        token_queue.pop();
                    }
                    cout<<" ";
                    //state = -4;
                }
                break;
            case 5:
                cout<<" state5:" ;
                if(s[i] == '+' || s[i] == '-')
                {
                    token_queue.push(s[i]);
                    i++;
                    state = 6;
                }
                else if(judge_type(s[i]) == 2)
                {
                    token_queue.push(s[i]);
                    i++;
                    state = 7;
                }
                else if(judge_type(s[i]) == 1)
                {
                    state = -6;/*
                    while(!token_queue.empty())
                    {
                        //cout<<"pop:";
                        if(token_queue.front() != 'e')
                            cout<<token_queue.front();
                        token_queue.pop();
                    }
                    cout<<" ";*/
                }
                else
                {
                    //token_queue.push('0');
                    //state = judge_type(s[i]);
                    state = -5;
                    while(!token_queue.empty())
                    {
                        //cout<<"pop:";
                        if(token_queue.front() != 'e' && token_queue.front() != '+' && token_queue.front() != '-')
                            cout<<token_queue.front();
                        token_queue.pop();
                    }
                    cout<<" ";
                }
                break;
            case 6:
                cout<<" state6:" ;
                if(judge_type(s[i]) == 2)
                {
                    token_queue.push(s[i]);
                    i++;
                    state = 7;
                }
                else
                {
                    state = 5;
                }
                break;
            case 7:
                cout<<" state7:" ;
                if(judge_type(s[i]) == 1)
                {
                    state = -6;/*
                    while(!token_queue.empty())
                    {
                        //cout<<"pop:";
                        cout<<token_queue.front();
                        token_queue.pop();
                    }
                    cout<<" ";*/
                }
                else if(judge_type(s[i]) == 2)
                {
                    state = 7;
                    token_queue.push(s[i]);
                    i++;
                }
                else if(s[i] == '.')
                {
                    state = 7;
                    i++;
                    //ERR
                }
                else
                {
                    state = judge_type(s[i]);
                    while(!token_queue.empty())
                    {
                        //cout<<"pop:";
                        cout<<token_queue.front();
                        token_queue.pop();
                    }
                    cout<<" ";
                }
                break;
            case 8://界符
                if(s[i] == '\'' || s[i] == '\"')//转义字符需要处理，不然字符串中的分号很可能被当作界符
                {
                    cout<<"state8:";
                    cout<<s[i]<<" ";

                    char crossing = s[i];
                    crossing_flag = true;
                    i++;
                    while(crossing_flag && s[i]!='\n')
                    {
                        if(s[i] == crossing)
                        {
                            crossing_flag = false;
                            i++;
                            state = judge_type(s[i]);
                            while(!token_queue.empty())
                            {
                                //cout<<"pop:";
                                cout<<token_queue.front();
                                token_queue.pop();
                            }
                            cout<<" ";
                            cout<<crossing<<" ";
                            
                        }
                        else if(s[i+1] == '\n')
                        {
                            crossing_flag = false;
                            token_queue.push(s[i]);
                            while(!token_queue.empty())
                            {
                                //cout<<"pop:";
                                cout<<token_queue.front();
                                token_queue.pop();
                            }
                            cout<<" ";
                            over = true;
                        }
                        else
                        {
                            token_queue.push(s[i]);
                            if(s[i] == '\\')
                            {
                                i++;
                                token_queue.push(s[i]);
                            }
                            i++;
                        }
                    }
                }
                else if(s[i] == '#')
                {
                    header_flag = true;
                    cout<<"header on";
                    cout<<"# ";
                    i++;
                    state = judge_type(s[i]);
                }
                else
                {
                    cout<<"state8:";
                    cout<<s[i]<<" ";
                    i++;
                    state = judge_type(s[i]);
                }
                break;
            case 9://运算符
                //cout<<"case 9:";
                if(s[i] == '/')
                {
                    i++;
                    if(s[i] == '*')
                    {
                        state = 10;
                        annotation = true;
                        i++;
                    }
                    else if(s[i] == '/')
                        over = true;
                    else if(judge_type(s[i]) == 9)
                    {
                        token_queue.push('/');
                        state = 9;
                    }
                    else
                    {
                        cout<<"/ ";
                        state = judge_type(s[i]);
                    }
                }/*
                else if(s[i] == '#')
                {
                    header_flag = true;
                    cout<<"header on";

                    i++;
                    state = judge_type(s[i]);
                }*/
                else if(s[i] == '<' && header_flag)
                {
                    cout<<"< (界符) ";
                    i++;
                    state = judge_type(s[i]);
                }
                else if(s[i] == '>' && header_flag)
                {
                    cout<<"> (界符) ";
                    header_flag = false;
                    i++;
                    state = judge_type(s[i]);
                }
                else
                {
                    if(judge_type(s[i+1]) == 9)
                    {
                        token_queue.push(s[i]);
                        i++;
                    }
                    else
                    {
                        token_queue.push(s[i]);
                        i++;
                        state = judge_type(s[i]);
                        while(!token_queue.empty())
                        {
                        //cout<<"pop:";
                            cout<<token_queue.front();
                            token_queue.pop();
                        }
                        cout<<" ";
                    }
                }
                
                break;
            case 10://进入注释模式
                cout<<"case 10:";
                if(s[i] == '\n')
                    over = true;
                else if(s[i] == '*')
                {
                    state = 11;
                    i++;
                }
                else
                {
                    i++;
                }
                
                break;
            case 11:
                cout<<"case 11:";
                if(s[i] == '/')
                {
                    annotation = false;
                    i++;
                    state = judge_type(s[i]);
                }
                else if(s[i] == '\n')
                    over = true;
                else
                {
                    state = 10;
                    i++;
                }
                break;
            case -3:
                i++;
                state = judge_type(s[i]);
                cout<<"(ERR3) ";
                break;
            case -4:
                over = true;
                break;
            case -5:
                state = judge_type(s[i]);
                cout<<"(ERR5) ";
                break;
            case -6://错误，非法标识符
                token_queue.push(s[i]);
                i++;
                if(judge_type(s[i]) == 1)
                    state = -6;
                else
                {
                    state = judge_type(s[i]);
                    while(!token_queue.empty())
                    {
                        //cout<<"pop:";
                        cout<<token_queue.front();
                        token_queue.pop();
                    }
                    cout<<"(ERR0) ";
                }
                break;
        }
    }
}

int main()
{
    read_op_table("./operator_map.txt");
    char s[] = "# include <iostream>\n";
    fstream t("lexical_analyzer.cpp",ios::in|ios::out);
    char temp[1024];
    while (t.getline(temp,1024))
    {
        string s1 = temp;
        //cout<<"hey";
        int len = s1.length();
        //cout<<len;
        temp[len] = '\n';
        //cout<<"hey";
        temp[len+1] = 0;
        analyze(temp);
        cout << endl;
        //cout<<"hey";
    }
    t.close();

    //tokens["&"] = 3;
    //read_op_table("./operator_map.txt");
    //cout<<" next state:"<<judge_type(s[16]);
    //cout<<" annotation: "<<annotation;
    //analyze(s);
    //cout<<operators["+"];
    //read_op_table("./temp.txt");
    /*
    map<string,int> temp;
    map<string,string> lex;
    temp["A"] = 1;
    temp["A"] ++;
    lex["A"] = "char";
    temp["B"]++;
    //cout<<temp["B"];
    map<string, int> ::iterator iter; 
    for(iter = temp.begin(); iter != temp.end(); iter++)
      cout << iter->first << " " << iter->second <<" "<<lex[iter->first]<< endl;
      */
    /*map<string, int> ::iterator iter; 
    for(iter = operators.begin(); iter != operators.end(); iter++)
        cout << iter->first << " " << iter->second << endl;*/
}

