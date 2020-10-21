# include <iostream>
# include <map>
# include <queue>
# include <string>
# include <fstream>

//# define EOF 0

using namespace std;

map<string,int> tokens;
queue<char> token_queue;

void read_op_table(string path)
{
    fstream t(path,ios::in|ios::out);
    char temp[1024];
    string out;
    while(t.getline(temp,1024))
    {
        out.append(temp);
        //out.append("\0");
    }
    cout<<out[1]<<endl;
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
    else
        return -2;
}

void analyze(char* s)
{
    int i = 0;
    int state = judge_type(s[i]);
    while(s[i] != 'z')
    {
        //state = judge_type(s[i]);
        //cout<<"in_while";
        switch(state)
        {
            case 0:
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
            case 3:
                i++;
                state = judge_type(s[i]);
                cout<<"ERR";
                break;
        }
    }
}

int main()
{
    char s[] = "abc ef&d sda __kjs z";
    tokens["&"] = 3;

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
}

