# include <iostream>
# include <map>
# include <queue>
# include <string>
# include <fstream>

//# define EOF 0

using namespace std;

map<string,int> tokens;//包含所有可识别的符号，可添加
map<int,string> token_type; //数字所代表的符号类型，固定
map<int,int> token_num;
queue<char> token_queue;

void read_op_table(string path)
{
    fstream t(path,ios::in|ios::out);
    char temp[1024];
    string out;
    while(t.getline(temp,1024))
    {
        int type = atoi(temp);
        t.getline(temp,1024);
        string s = temp;
        tokens[s] = type;
         //cout<<s.length();

        //out.append("\0");
    }
    t.close();
}

int judge_type(char c)
{
    char temp_c[2];
    temp_c[0] =  c;

    if(c == ' ')
        return 0;
    else if(c == '_' || (c >= 'a' && c <= 'z') || (c>='A' && c <= 'Z'))
        return 1;
    else if(c>='0' && c<='9')
        return 2;
    else if(tokens[temp_c])
        return tokens[temp_c];
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
                i++;
                state = judge_type(s[i]);
                break;
            case 1:
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
                    token_queue.push('0');
                    state = -6;
                    while(!token_queue.empty())
                    {
                        //cout<<"pop:";
                        cout<<token_queue.front();
                        token_queue.pop();
                    }
                    cout<<" ";
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
                    state = -6;
                    while(!token_queue.empty())
                    {
                        //cout<<"pop:";
                        cout<<token_queue.front();
                        token_queue.pop();
                    }
                    cout<<" ";
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
                    state = -6;
                    while(!token_queue.empty())
                    {
                        //cout<<"pop:";
                        if(token_queue.front() != 'e')
                            cout<<token_queue.front();
                        token_queue.pop();
                    }
                    cout<<" ";
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
                    state = -6;
                    while(!token_queue.empty())
                    {
                        //cout<<"pop:";
                        cout<<token_queue.front();
                        token_queue.pop();
                    }
                    cout<<" ";
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
            case -6:
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
    char s[] = "3. 3. 4. 5eA\n";
    
    //tokens["&"] = 3;
    analyze(s);
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
    for(iter = tokens.begin(); iter != tokens.end(); iter++)
        cout << iter->first << " " << iter->second << endl;*/
}

