# include <iostream>
# include <map>
# include <queue>
# include <string>
# include <fstream>

# define OPERATOR_MAP_PATH "./doc/operator_map.txt"
# define TOKEN_TABLE_PATH "./doc/tokens.txt"
# define TOKEN_MAP_PATH "./doc/token_map.txt"

//# define EOF 0

using namespace std;

map<string,int> tokens;//包含所有可识别的符号及保留字，可添加
map<int,string> token_type; //数字所代表的符号类型，固定
map<int,int> token_num;//用以统计各类型记号的出现次数
map<string, int> operators;//存储单词首字母与初入状态间的映射关系
queue<char> token_queue;
string err_info;
int line = 0;
int number_num = 0;

bool header_flag = false;
bool crossing_flag = false;
bool annotation = false;
bool complete_info = false;
bool show_line_num = true;

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

void read_tokens()
{
    fstream t(TOKEN_TABLE_PATH,ios::in|ios::out);
    char temp[1024];
    while(t.getline(temp,1024))
    {
        int type = atoi(temp);
        //cout<<"get:"<<type<<endl;
        t.getline(temp,1024);
        string s = temp;
        tokens[s] = type;
         //cout<<s.length();

        //out.append("\0");
    }
    t.close();
}

void read_token_map()
{
    fstream t(TOKEN_MAP_PATH,ios::in|ios::out);
    char temp[1024];
    for(int i = 1; i <= 11; i++)
    {
        //cout<<"get:"<<type<<endl;
        t.getline(temp,1024);
        string s = temp;
        token_type[i] = s;
    }
}

void print_token_table()
{
    map<string, int> ::iterator iter;
    for(iter = tokens.begin(); iter != tokens.end(); iter++)
    {
        if(iter->second == 1)
            cout << '<' << iter->first << " , " << token_type[iter->second] << '>' << endl;
        else
            cout << '<' << token_type[iter->second] << " , " << iter->first << '>' << endl;
    }
}

void print_statistics()
{
    int token_total = 0;
    cout<<endl<<"[数据统计]:"<<endl;
    for(int i = 1; i <= 11; i++)
    {
        cout<<token_type[i]<<": "<<token_num[i]<<'\t';
        token_total += token_num[i];
    }
    cout<<"常数: "<<number_num<<endl<<endl;
    cout<<"<总计>有效记号数: "<<token_total<<"\t"<<"行数: "<<line<<endl<<endl;
}

void print_err_info()
{
    cout<<endl<<"[错误信息]:"<<endl;
    cout<<err_info<<endl;
}

void print_str(string s)
{
    cout << "<" << "字符串"<<" , "<< s << ">" <<endl;
}

void string_inc()
{
    token_num[11]++;
}

void print_token(string s)
{
    if(tokens[s] == 1)
    {
        cout << '<' << s << " , " << token_type[1] << '>' << endl;
    }
    else if(tokens[s] > 1)
    {
        cout << '<' << token_type[tokens[s]] << " , " << s << '>' << endl;
    }
    else
        cout << "<" << "标识符"<<" , "<< s << ">" <<endl;
}

void print_num(string s)
{
    cout<< "<" << "常数" << " , " << s << ">" <<endl;
}

bool token_in_table(string s)
{
    if(tokens[s])
        return true;
    else
        return false;
}

string generate_token()
{
    string temp;
    while(!token_queue.empty())
    {            //cout<<"pop:";
        temp += token_queue.front();
        token_queue.pop();
    }
    return temp;
}
void add_token(string s)//将记号添加到记号表
{
    tokens[s] = 10;
}

void token_inc(string s)//该类型记号数量++
{
    token_num[tokens[s]]++;
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
                if(complete_info)cout<<"==>state0:";
                i++;
                state = judge_type(s[i]);
                //cout<<"next:"<<state;
                break;
            case 1:
                if(complete_info)cout<<"==>state1:";
                token_queue.push(s[i]);
                //cout<<"pushed:"<<s[i]<<endl;
                i++;
                if(judge_type(s[i]) == 1 || judge_type(s[i]) == 2)
                {
                    state = 1;
                }
                else
                {
                    /*
                    while(!token_queue.empty())
                    {
                        //cout<<"pop:";
                        cout<<token_queue.front();
                        token_queue.pop();
                    }
                    cout<<" ";*/
                    string str  = generate_token();
                    if(!token_in_table(str))
                        add_token(str);
                    print_token(str);
                    token_inc(str);
                    state = judge_type(s[i]);
                    //cout<<"exit_state:"<<state<<" ";
                }
                break;
            case 2:
                //cout<<"2->"<<"pushed:"<<s[i]<<'~';
                if(complete_info)cout<<"==>state2:" ;
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
                    /*
                    while(!token_queue.empty())
                    {
                        //cout<<"pop:";
                        cout<<token_queue.front();
                        token_queue.pop();
                    }
                    cout<<" ";*/
                    number_num++;
                    string str = generate_token();
                    print_num(str);
                }
                
                break;
            case 3://识别到小数点
                //cout<<"3->";
                if(complete_info)cout<<"==>state3:" ;
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
                    string temp_err;
                    temp_err = "<WARNING>Line" +  to_string(line) + ": Multiple float points found, has recovered by dropping redundant ones.\n";
                    err_info += temp_err;
                    //ERR:出现多个连续小数点
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
                    state = -5;//缺省错误
                    /*
                    while(!token_queue.empty())
                    {
                        //cout<<"pop:";
                        cout<<token_queue.front();
                        token_queue.pop();
                    }
                    cout<<" ";*/
                    string str = generate_token();
                    print_num(str);
                    number_num ++;

                    string temp_err;
                    temp_err = "<WARNING>Line" +  to_string(line) + ": Digit missing after float point, has taken as default \"" + str + "\"\n";
                    err_info += temp_err;
                }
                break;
            case 4:
                if(complete_info)cout<<"==>state4:" ;
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
                else if(s[i] == '.')//小数点后又出现小数点，仅第一个小数点有效
                {
                    i++;
                    state = 4;

                    string temp_err;
                    temp_err = "<WARNING>Line" +  to_string(line) + ": Multiple float points found, has recovered by dropping redundant ones.\n";
                    err_info += temp_err;
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
                    /*
                    while(!token_queue.empty())
                    {
                        //cout<<"pop:";
                        cout<<token_queue.front();
                        token_queue.pop();
                    }
                    cout<<" ";*/
                    string str = generate_token();
                    print_num(str);
                    number_num++;
                    //state = -4;
                }
                break;
            case 5:
                if(complete_info)cout<<"==>state5:" ;
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
                    string str;
                    while(!token_queue.empty())
                    {
                        //cout<<"pop:";
                        if(token_queue.front() != 'e' && token_queue.front() != '+' && token_queue.front() != '-')
                            str += token_queue.front();
                        token_queue.pop();
                    }
                    //cout<<" ";
                    string temp_err;
                    temp_err = "<WARNING>Line" +  to_string(line) + ": Digit missing after 'e', has taken as default \"" + str + "\"\n";
                    err_info += temp_err;

                    print_num(str);
                    number_num++;
                }
                break;
            case 6://1e+xxx
                if(complete_info)cout<<"==>state6:" ;
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
            case 7://1e-1xx
                if(complete_info)cout<<"==>state7:" ;
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
                else if(s[i] == '.')//1e-5之后又出现小数点，忽略
                {
                    state = 7;
                    i++;

                    string temp_err;
                    temp_err = "<WARNING>Line" +  to_string(line) + ": Multiple float points found, has recovered by dropping redundant ones.\n";
                    err_info += temp_err;
                    //ERR
                }
                else
                {
                    state = judge_type(s[i]);/*
                    while(!token_queue.empty())
                    {
                        //cout<<"pop:";
                        cout<<token_queue.front();
                        token_queue.pop();
                    }
                    cout<<" ";*/
                    string str = generate_token();
                    print_num(str);
                    number_num++;
                }
                break;
            case 8://界符
                if(complete_info)cout<<"==>state8:";
                if(s[i] == '\'' || s[i] == '\"')//转义字符需要处理，不然字符串中的分号很可能被当作界符
                {
                    //cout<<s[i]<<" ";
                    if(!token_queue.empty())
                    {
                        string str = generate_token();
                        if(token_in_table(str))
                        {
                            print_token(str);
                            token_inc(str);
                        }
                        else
                        {
                            string temp_err;
                            temp_err = "<ERROR>Line" +  to_string(line) + ": Unsupported operator \""+ str +"\"\n";
                            err_info += temp_err;
                            //cout<<"unsupported operator \""<<str<<"\""<<endl;
                        }
                    }

                    token_queue.push(s[i]);
                    string str = generate_token();
                    token_inc(str);
                    print_token(str);

                    char crossing = s[i];
                    crossing_flag = true;
                    i++;

                    if(s[i]=='\n')
                    {
                        over = true;
                        crossing_flag = false;
                        string temp_err;
                        temp_err = "<WARNING>Line" +  to_string(line) + ": Quotation mark missing, has taken all content after the first quotation mark as a string.\n";
                        err_info += temp_err; 
                    }   

                    while(crossing_flag && s[i]!='\n')
                    {
                        if(s[i] == crossing)
                        {
                            crossing_flag = false;
                            string str = generate_token();
                            if(str != "")
                            {
                                string_inc();
                                print_str(str);
                            }
                            /*
                            while(!token_queue.empty())
                            {
                                //cout<<"pop:";
                                cout<<token_queue.front();
                                token_queue.pop();
                            }
                            cout<<" ";*/
                            token_queue.push(s[i]);
                            str = generate_token();
                            token_inc(str);
                            print_token(str);
                            //cout<<crossing<<" ";
                            i++;
                            state = judge_type(s[i]);
                        }
                        else if(s[i+1] == '\n')//到行末也没有出现引号
                        {
                            crossing_flag = false;

                            string temp_err;
                            temp_err = "<WARNING>Line" +  to_string(line) + ": Quotation mark missing, has taken all content after the first quotation mark as a string.\n";
                            err_info += temp_err;

                            token_queue.push(s[i]);/*
                            while(!token_queue.empty())
                            {
                                //cout<<"pop:";
                                cout<<token_queue.front();
                                token_queue.pop();
                            }
                            cout<<" ";*/
                            string str = generate_token();
                            string_inc();
                            print_str(str);

                            over = true;
                        }/*
                        else if(s[i+1] == '\n' && s[i+2] != '\0')//字符串中有换行符，但未到代码行末
                        {
                            token_queue.push(s[i]);
                            i++;
                            token_queue.push(s[i]);
                            i++;
                        }*/
                        else
                        {
                            token_queue.push(s[i]);
                            if(s[i] == '\\')//处理转义引号的情况
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
                    //cout<<"header on";
                    token_queue.push(s[i]);
                    string str = generate_token();
                    token_inc(str);
                    print_token(str);

                    //cout<<"# ";
                    i++;
                    state = judge_type(s[i]);
                }
                else
                {
                    //if(complete_info)cout<<"==>state8:";
                    token_queue.push(s[i]);
                    string str = generate_token();
                    token_inc(str);
                    print_token(str);
                    //cout<<s[i]<<" ";
                    i++;
                    state = judge_type(s[i]);
                }
                break;
            case 9://运算符
                if(complete_info)cout<<"==>state 9:";
                if(s[i] == '/')
                {
                    i++;
                    if(s[i] == '*')
                    {         
                        if(!token_queue.empty())
                        {
                            string str = generate_token();
                            if(token_in_table(str))
                            {
                                print_token(str);
                                token_inc(str);
                            }
                            else
                            {
                                string temp_err;
                                temp_err = "<ERROR>Line" +  to_string(line) + ": Unsupported operator \""+ str +"\"\n";
                                err_info += temp_err;
                                //cout<<"unsupported operator \""<<str<<"\""<<endl;
                            }
                        }

                        state = 10;
                        annotation = true;
                        //cout<<"turned true";
                        i++;
                    }
                    else if(s[i] == '/')
                    {
                        over = true;
                        if(!token_queue.empty())
                        {
                            string str = generate_token();
                            if(token_in_table(str))
                            {
                                print_token(str);
                                token_inc(str);
                            }
                            else
                            {
                                string temp_err;
                                temp_err = "<ERROR>Line" +  to_string(line) + ": Unsupported operator \""+ str +"\"\n";
                                err_info += temp_err;
                                //cout<<"unsupported operator \""<<str<<"\""<<endl;
                            }
                        }
                    }
                    else if(judge_type(s[i]) == 9)
                    {
                        token_queue.push('/');
                        state = 9;
                    }
                    else
                    {
                        //cout<<"/ ";
                        token_queue.push('/');
                        string str = generate_token();
                        token_inc(str);
                        print_token(str);
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
                    //cout<<"< (界符) ";
                    token_num[2]++;
                    cout<<"<"<<"界符"<<" , "<<"<>"<<endl;
                    i++;
                    state = judge_type(s[i]);
                }
                else if(s[i] == '>' && header_flag)
                {
                    //cout<<"> (界符) ";
                    token_num[2]++;
                    cout<<"<"<<"界符"<<" , "<<">>"<<endl;
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
                        state = judge_type(s[i]);/*
                        while(!token_queue.empty())
                        {
                        //cout<<"pop:";
                            cout<<token_queue.front();
                            token_queue.pop();
                        }
                        cout<<" ";*/
                        string str = generate_token();
                        if(token_in_table(str))
                        {
                            print_token(str);
                            token_inc(str);
                        }
                        else
                        {
                            string temp_err;
                            temp_err = "<ERROR>Line" +  to_string(line) + ": Unsupported operator \""+ str +"\"\n";
                            err_info += temp_err;
                            //cout<<"unsupported operator \""<<str<<"\""<<endl;
                        }
                        
                    }
                }
                
                break;
            case 10://进入注释模式
                if(complete_info)cout<<"==>state 10:";
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
                if(complete_info)cout<<"==>state 11:";
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
                if(complete_info)cout<<"==>state -3: ";
                break;
            case -4://退出状态
                over = true;
                if(complete_info)cout<<"==>state -4: ";
                break;
            case -5:
                state = judge_type(s[i]);
                if(complete_info)cout<<"==>state -5: ";
                break;
            case -6://错误，非法标识符
                if(complete_info)cout<<"==>state -6: ";
                token_queue.push(s[i]);
                i++;
                if(judge_type(s[i]) == 1)
                    state = -6;
                else
                {
                    state = judge_type(s[i]);
                    string temp_str;
                    while(!token_queue.empty())
                    {
                        //cout<<"pop:";
                        temp_str += token_queue.front();
                        token_queue.pop();
                    }
                    string temp_err;
                    temp_err = "<ERROR>Line" +  to_string(line) + ": Unsupported identifier \"" + temp_str + "\"\n";
                    err_info += temp_err;
                    //cout<<"(ERR0) ";
                }
                break;
        }
    }
}

void print_readme()
{
    fstream t("./readme.txt",ios::in|ios::out);
    char temp[1024];
    string s;
    while (t.getline(temp,1024))
    {
        s.append(temp);
        s.append("\n");
    }
    cout<<s<<endl;
}

int main(int argc, char *argv[])
{
    read_op_table(OPERATOR_MAP_PATH);
    read_tokens();
    read_token_map();
    string code_path;

    if(argc > 1)
    {
        for(int i = 1; i < argc ; i++)
        {
            string s = argv[i];
            if(s.compare("analyze") == 0)
            {
                i++;
                code_path = argv[i];
            }
            else if(s.compare("-o") == 0)
            {
                i++;
                freopen(argv[i],"w",stdout);
            }
            else if(s.compare("--pure") == 0)
            {
                show_line_num = false;
            }
            else if(s.compare("--complete") == 0)
            {
                show_line_num = true;
                complete_info = true;
            }
            else if(s.compare("-h") == 0)
            {
                print_readme();
            }
            else if(s.compare("token-table") == 0)
            {
                print_token_table();
            }
        }
    }
    else
    {
        print_readme();
    }
    
    
    //freopen("./output.txt","w",stdout);
    //print_token_table();
    
    /*
    line++;
    complete_info = true;
    char c[] = "a +++\"+ 1234basdsc\\n\"\n";
    analyze(c);
    print_err_info();*/


    //print_statistics();
    
    //char s[] = "# include <iostream>\n";
    fstream t(code_path,ios::in|ios::out);
    char temp[1024];
    while (t.getline(temp,1024))
    {
        line++;
        if(show_line_num)
            cout<<"Line:"<<line<<endl;
        string s1 = temp;
        //cout<<"hey";
        int len = s1.length();
        //cout<<len;
        temp[len] = '\n';
        //cout<<"hey";
        temp[len+1] = 0;
        analyze(temp);
        if(show_line_num)
            cout << endl;
        if(complete_info)
            cout << endl;
        //cout<<"hey";
    }
    t.close();
    //print_err_info();
    //print_statistics();
    //print_token_table();
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
    system("pause");
}