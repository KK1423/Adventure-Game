#include <iostream>
#include<math.h>
#include<stdlib.h>
#include<string>
using namespace std;
bool defaultItemFunction()
{
    //cout<<"Wait... what??"<<endl;
    return false;
}
class item
{
public:
    item *nextitem = 0;
    float strength;
    bool (*usefunction)(void) = defaultItemFunction;
    string name;
    item (string s , float st)
    {
        name = s;
        strength = st;
    }
};

/*class option
{
public:
    bool itemorattack = false;//item
    int itemindex = 0;
    bool (*exec)(bool,int);
    option *nextoption = 0;
};*/
class room;
class enemy
{
public:
    string name;
    float HP;
    float attackStrength = 5;
    float attackProbability = 3;
    float Variance = 2;
    item *dropitem = 0;
    enemy *lastEnemy = 0;
    enemy *nextenemy = 0;
    room *inroom;
    void die();
    // enemy(float hp, string naam,room *inrom,item *droitem);
    enemy(float hp, string naam,room *inrom,item *droitem, float attackStength,float attackprob,float v);
};
class room
{
public:
    int roomid;
    int linkcache = 0;
    bool old = false;
    string roomname;
    string newroomname = "@";
    string newenterstring = "@";
    string newfailstring = "@";
    room *connectedrooms[4] = {0,0,0,0};
    bool (*enterfunction)(room*);
    //option *startoption;
    enemy *startenemy = 0;
    item *startroomitem = 0;
    room(int id,string name);
};
void enemy::die()
{
    if(lastEnemy == 0)
    {
        inroom->startenemy = nextenemy;
    }
    else
    {
        lastEnemy->nextenemy = nextenemy;
    }
    item*traverse = inroom->startroomitem;
    if(traverse == 0)
    {
        inroom->startroomitem = dropitem;
    }
    else
    {
        while(traverse->nextitem!=0)
        {
            traverse = traverse->nextitem;
        }
        traverse->nextitem = dropitem;
    }
    delete this;
}
enemy::enemy(float hp, string naam,room *inrom,item *droitem, float attackStength, float attackprob, float v)
{
    HP=hp;
    attackStrength = attackStength;
    attackProbability = attackprob;
    name = naam;
    Variance = v;
    dropitem = droitem;
    if(droitem!=0)droitem->nextitem = 0;
    enemy *traverse = inrom->startenemy;
    inroom = inrom;
    if(traverse == 0)
    {
        inroom->startenemy = this;
        return;
    }
    while(traverse->nextenemy!=0)
    {
        traverse = traverse->nextenemy;
    }
    traverse->nextenemy = this;
    lastEnemy = traverse;
}
string decaps(string n)
{
    for(int x = 0; x<n.length(); x++)
    {
        if((int)n[x]>=65&&(int)n[x]<=90)
        {
            n[x] = char((int)n[x]+32);
        }
    }
    return n;
}
room *startroom;
room *currentroom;
float HP = 20;
item *invstart = 0;
item *holding = 0;
bool winflag = false;
bool roomContainsEnemy(string name)
{
    enemy *traverse = currentroom->startenemy;
    if(traverse == 0)
    {
        return false;
    }
    while(traverse!=0)
    {
        if(decaps(traverse->name) == decaps(name))
        {
            return true;
        }
        traverse = traverse->nextenemy;
    }
    return false;
}
enemy* getEnemy(string searchString)
{
    enemy* traverse = currentroom->startenemy;
    if(traverse == 0){return 0;}
    while(!(decaps(traverse->name)==decaps(searchString)))
    {

        traverse = traverse->nextenemy;
        if(traverse == 0) return 0;
    }
    return traverse;
}
void inventoryAdd(item*i)
{
    item *traverse = invstart;
    if(traverse == 0)
    {
        invstart = i;
        return;
    }
    while(traverse->nextitem != 0)
    {
        traverse = traverse->nextitem;
    }
    traverse->nextitem = i;
}
item* inventoryRemove(string name)
{
    item *lastitem;
    item *traverse  = invstart;
    if(decaps(traverse->name) == decaps(name))
    {
        invstart = traverse->nextitem;
        traverse->nextitem = 0;
        return traverse;
    }
    while(traverse != 0)
    {
        if(decaps(traverse->name) == decaps(name))
        {
            lastitem->nextitem = traverse->nextitem;
            traverse->nextitem = 0;
            return traverse;
        }
        lastitem = traverse;
        traverse = traverse->nextitem;
    }
    return 0;
}
bool inventoryContains(string name)
{
    item *traverse = invstart;
    if(traverse == 0)
    {
        return false;
    }
    while(traverse!=0)
    {
        if(decaps(traverse->name) == decaps(name))
        {
            return true;
        }
        traverse = traverse->nextitem;
    }
    return false;
}
item* lastInventoryItem()
{
    item* traverse = invstart;
    while(traverse->nextitem != 0)
    {
        traverse = traverse->nextitem;
    }
    return traverse;
}
item* roomRemove(string name)
{
    item *lastitem;
    item *traverse  = currentroom->startroomitem;
    if(decaps(traverse->name) == decaps(name))
    {
        currentroom->startroomitem = traverse->nextitem;
        traverse ->nextitem = 0;
        return traverse;
    }
    while(traverse != 0)
    {
        if(decaps(traverse->name) == decaps(name))
        {
            lastitem->nextitem = traverse->nextitem;
            traverse->nextitem = 0;
            return traverse;
        }
        lastitem = traverse;
        traverse = traverse->nextitem;
    }
    return 0;
}
bool roomContains(string name)
{
    item *traverse = currentroom->startroomitem;
    if(traverse == 0)
    {
        return false;
    }
    while(traverse!=0)
    {
        if(decaps(traverse->name) == decaps(name))
        {
            return true;
        }
        traverse = traverse->nextitem;
    }
    return false;
}

/*void linkrooms(room *room1,room *room2)
{

}*/
bool defaultfunction (room* roominstance)
{
    return true;
}
room::room(int id,string name)
{
    roomid = id;
    roomname = name;
    enterfunction = defaultfunction;
}
bool atticEnter (room* roominstance)
{
    if(!inventoryContains("key"))return false;
    delete inventoryRemove("key");
    return true;
}
bool bedroomEnter (room* roominstance)
{
    if(inventoryContains("rope"))
    {
       // cout<<"You stick the key in the lock, but you somehow accidentally swallow it."<<endl<<"Pathetic."<<endl;
        delete inventoryRemove("rope");
        return true;
    }
    return false;
}
bool Potion()
{
    static int uses = 3;
    uses--;
    HP+=10;
    cout<<"The potion gave you 10 more health points"<<endl;
    if(uses==0)
    {
        cout<<"You used up all the potion"<<endl;
        delete holding;
        holding = 0;
    }
    return true;
}
bool Rune()
{
    HP = 0;
    winflag = true;
    return true;
}
void init()
{
    /* Note: The storyline was created by Mrs.Heyboars CAP class        */
    room *rooms[6] =
    {
        new room(1,"Basement"),
        new room(2,"Living Room"),
        new room(3,"Attic"),
        new room(4,"Hall"),
        new room(5,"Bathroom"),
        new room(6,"Bedroom")
    };
    rooms[2]->enterfunction = atticEnter;
    rooms[5]->enterfunction = bedroomEnter;
    rooms[0]->connectedrooms[1] = rooms[1];
    rooms[1]->connectedrooms[0] = rooms[0];
    rooms[1]->connectedrooms[1] = rooms[2];
    rooms[1]->connectedrooms[2] = rooms[3];
    rooms[2]->connectedrooms[0] = rooms[1];
    rooms[3]->connectedrooms[0] = rooms[1];
    rooms[3]->connectedrooms[1] = rooms[4];
    rooms[3]->connectedrooms[2] = rooms[5];
    rooms[4]->connectedrooms[0] = rooms[3];
    rooms[5]->connectedrooms[0] = rooms[3];
    startroom = rooms[0];
    rooms[0]->newroomname = "@";
    rooms[1]->newroomname = "Climb ladder";
    rooms[2]->newroomname = "Climb ladder";
    rooms[3]->newroomname = "Keep going";
    rooms[4]->newroomname = "Left door";
    rooms[5]->newroomname = "Right door";
    rooms[0]->newenterstring = "You wake up in a cold, dusty cellar";
    rooms[1]->newenterstring = "You find a luxurious living area";
    rooms[2]->newenterstring = "You use the key to open the door. You smell cheese as you are thrown to the floor";
    rooms[3]->newenterstring = "You look down a short hallway";
    rooms[4]->newenterstring = "You enter a small, but expensive bathroom";
    rooms[5]->newenterstring = "You use the rope to enter a bedroom, because game logic";
    rooms[2]->newfailstring = "You need a Key";
    rooms[5]->newfailstring = "You need a Key";
    item *items[] =
    {
        new item("Knife",3),
        new item("Rope", 0),
        new item("Sword", 6),
        new item("Key",0),
        new item("Bedroomkey",0),
        new item("HealthPotion",-10),
        new item("Rune",0)
    };
    rooms[0]->startroomitem = items[0];
    items[0]->nextitem = items[1];
    rooms[1]->startroomitem  = items[2];
    //rooms[4]->startroomitem  = items[4];
    rooms[5]->startroomitem  = items[3];
    rooms[3]->startroomitem  = items[5];
    rooms[2]->startroomitem = items[6];
    items[5]->usefunction = Potion;
    items[6]->usefunction = Rune;
    //holding = items[1];
    enemy *enemies[] =
    {
        new enemy(20,"The Rubber Ducky",rooms[4],items[4],4,3,2),
        new enemy(50,"The Pepperoni Pizza",rooms[2],0,6,2,3)
    };
}
void printNavChoices()
{
    int n =0;
    cout<<"GO TO"<<endl;
    while(n<=3)
    {
        if(currentroom->connectedrooms[n]!=0)
        {
            if(currentroom->connectedrooms[n]->old)
            {
                cout<<"     "<<" "<<(currentroom->connectedrooms[n])->roomname<<endl;
            }
            else
            {
                cout<<"     "<<" "<<(currentroom->connectedrooms[n])->newroomname<<endl;
            }
        }
        n++;
    }
    cout<<endl;
    if(currentroom->startroomitem!=0)
        cout<<"PICK UP"<<endl;
    item *ipointer = currentroom->startroomitem;
    while(ipointer != 0)
    {
        cout<<"     "<<ipointer->name<<endl;
        ipointer = ipointer->nextitem;
    }
    cout<<endl;
    if(currentroom->startenemy!=0)
        cout<<"ENEMIES"<<endl;
    enemy *epointer = currentroom->startenemy;
    while(epointer != 0)
    {
        cout<<"     "<<epointer->name<<endl;
        cout<<"          HP:"<<epointer->HP<<endl;
        epointer = epointer->nextenemy;
    }

}
void printInventory()
{
    item* traverse = invstart;
    int totalLength = 0;
    if(traverse == 0)return;
    for(int a = 0; a<40; a++)
    {
        cout<<"--";
    }
    cout<<endl;
    while(traverse!=0)
    {
        if(totalLength >= 80)
        {
            cout<<endl;
            totalLength = 0;
        }
        cout<<traverse->name<<" ";
        totalLength+= traverse->name.length();
        traverse = traverse->nextitem;
    }
    cout<<endl;
    for(int a = 0; a<20; a++)
    {
        cout<<"----";
    }
    cout<<endl<<"HP:"<<HP<<endl;
}
void printItemChoices()
{
}
int type = 0;
string getSecondWord(string s,bool direction)
{
    int n = 0;
    //cout<<s.size()<<endl;
    while((int)s[n] != 32)
    {
        if(n==s.size())
        {
            return"@";
        }
        n++;
        // cout<<s[n]<<" "<<endl;
    }
    return direction?s.substr(n+1):s.substr(0,n+1);
}
//int type = 0;
void updateEnemies()
{
    float HPdrain;
    enemy *traverse = currentroom->startenemy;
    while(traverse!=0)
    {
        srand(rand());
        if(rand()%(int)traverse->attackProbability == 0)
        {
            HPdrain = traverse->attackStrength +(traverse->Variance*(rand()%10)/10.0)*(rand()&1?1:-1);
            HP-=HPdrain;
            cout<<"The "<< traverse->name<<" attacked, and you lost "<<HPdrain<<" HealthPoints"<<endl;
        }
        traverse = traverse->nextenemy;
    }
}
string numberToString(int n)
{
    n = abs(n);
    int a =1;
    int e =0;
    while(n>a)
    {
        a*=10;
        e++;
    }
    if(n==a)e++;
    char *out = new char[e+1];
    out[e]=(char)0;
    int i = e-1;
    while(i>=0)
    {
        int j = n%10;
        n=n/10;
        out[i]=(char) (j+48);
        i--;
    }
    string finalstring(out);
    return finalstring;
}

string* parse(string s)
{
    while(s[s.length()-1]==' ')
    {
        s = s.substr(0,s.length()-1);
    }
    while(s[0]==' ')
    {
        s = s.substr(1);
    }
    string c(s);
    int spaceCount = 0;
    while(c.length()>0)
    {
        if((int)c[0]==32)
        {
            spaceCount++;
            while((int)c[0]==32)
            c = c.substr(1);
        }else
        c = c.substr(1);
    }
    c = s;
    string * outwords = new string[spaceCount+2]();
    outwords[spaceCount+1]="@";
    int currentwordindex = 0;
    int characterindex = -1;
    while(c.length()>0)
    {
        characterindex++;
        if(c[characterindex]==32)
        {
            outwords[currentwordindex] = c.substr(0,characterindex);
            c = c.substr(characterindex+1);
            currentwordindex++;
            while(c[0]==32)
            {
                c = c.substr(1);
            }
            characterindex = -1;
        }

            if(currentwordindex == spaceCount){outwords[currentwordindex] = c;return outwords;}
    }
    return outwords;
}
string ignorewords[] =
{
"using",
"the",
"to",
"on",
"into",
"up",
"down"
,"@"
};
string pickupwords[] =
{
"Pick",
"take",
"grab",
"get"
,"@"
};
string movewords[] =
{
"walk",
"run",
"move",
"go",
"@"
};
string attackwords[] =
{
"attack",
"assail",
"kick",
"hit",
"bash",
"punch",
"strike",
"@"
};
string holdwords[] =
{
    "hold",
    "with",
    "@"
};
bool contains(string keyword,string* group,int listlength = 1e10)
{
    int i = 0;
    while(i<listlength&&group[i]!="@")
    {
        if(decaps(keyword)==decaps(group[i]))
            return true;
        i++;
    }
    return false;
}//                   input           searching for
bool contains(string* keyword,string* searchwords)
{
    int a = 0,b = 0;
    while(keyword[a]!="@"&&searchwords[b]!="@")
    {
        cout<<keyword[a]<<"|"<<searchwords[b]<<endl;
        if(decaps(keyword[a])==decaps(searchwords[b]))
        {
            b++;
            a++;
        }
        else
        {
            a++;
        }
    }
    if(searchwords[b]=="@")
    {
        return true;
    }else
    {
        return false;
    }
}
void doskips(string* input,string* wordslist)
{
    int i = 0;
    int offset = 0;
    while(input[i+offset]!="@")
    {
        while(contains(input[i+offset],wordslist))
            offset++;
        input[i]= input[i+offset];
        if(input[i+offset]!="@")i++;
    }
    input[i] = "@";
    i = 0;
    int j = 0;
    while(input[i]!="@")
    {
        while(j<input[i].length())
        {
            if(input[i][j]==44||input[i][j]==46)
                input[i] = input[i].substr(0,j);
            j++;
        }
        i++;
    }

}
string* getoptionwords(room* inroom, int i = 0)
{
    room* nextroom = inroom->connectedrooms[i];
    if(nextroom==0)
    {
        return 0;
    }
    return parse(nextroom->old? nextroom->roomname:nextroom->newroomname);
}
item* checkforpick(string* input,room inroom)
{
    item* traverse = inroom.startroomitem;
    int i =0;
    bool flag = false;
    while(input[i]!="@")
    {
        if(contains(input[i],pickupwords))
            flag = true;
        i++;
    }
    if(!flag)return 0;
    item * lastone = 0;
    while(traverse!=0)
    {
        string* parseditemname = parse(traverse->name);
        if(contains(input,parseditemname))
        {
            if(lastone == 0)
            {
                currentroom->startroomitem = traverse->nextitem;
            }else
            {
                lastone->nextitem = traverse->nextitem;
            }
            traverse->nextitem = 0;
            return traverse;
        }
        lastone = traverse;
        traverse = traverse->nextitem;
        delete[] parseditemname;
    }
    return new item("@",0);
}
void doholds(string* input,room inroom)
{
    item* traverse = invstart;
    int i =0;
    bool flag = false;
    while(input[i]!="@")
    {
        if(contains(input[i],holdwords))
            flag = true;
        i++;
    }
    if(!flag)return;
    item * lastone = 0;
    while(traverse!=0)
    {
        string* parseditemname = parse(traverse->name);
        if(contains(input,parseditemname))
        {
            if(lastone == 0)
            {
                invstart = traverse->nextitem;
            }else
            {
                lastone->nextitem = traverse->nextitem;
            }
            traverse->nextitem = 0;
            if(holding!=0)
            holding->nextitem = 0;
            inventoryAdd(holding);
            holding = traverse;
            return;
        }
        lastone = traverse;
        traverse = traverse->nextitem;
        delete[] parseditemname;
    }
    return;
}
void doattacks(string* input, room inroom)
{
    enemy* traverse = inroom.startenemy;
    int i =0;
    bool flag = false;
    while(input[i]!="@")
    {
        if(contains(input[i],attackwords))
            flag = true;
        i++;
    }
    if(!flag)(return;)
    item * lastone = 0;
    while(traverse!=0)
    {
        string* parseditemname = parse(traverse->name);
        if(contains(input,parseditemname))
        {
            if(lastone == 0)
            {
                currentroom->startroomitem = traverse->nextitem;
            }else
            {
                lastone->nextitem = traverse->nextitem;
            }
            traverse->nextitem = 0;
            return traverse;
        }
        lastone = traverse;
        traverse = traverse->nextenemy;
        delete[] parseditemname;
    }
}
int checkformove(string* input,room inroom)
{
    string* parsedoption;
    doskips(input,movewords);
    for(int i = 0; i<=3;i++)
    {
        if(inroom.connectedrooms[i]!=0)
        {
            parsedoption = parse(inroom.connectedrooms[i]->old? inroom.connectedrooms[i]->roomname:inroom.connectedrooms[i]->newroomname);
            int n =0;
            bool check = contains(input,parsedoption);
           /*while(parsedoption[n]!="@"&&input[n]!="@")
            {
                check = check&&decaps(parsedoption[n])==decaps(input[n]);
                n++;
            }
            delete parsedoption;
            if(n==0)check=false;*/
            if(check)return i;
        }

    }
    return 5;

}
string* words;
string getString()
{
    string s = "@";
    getline(cin,s);
    //                 cout<<s<<endl;
        for(int n =0;n<30;n++)
            cout<<endl;
    words = parse(s);
    doskips(words,ignorewords);
    int i =0;
    while(words[i]!="@")
    {
        cout<<(string)words[i]<<endl;
        i++;
    }
    item* pickitem = checkforpick(words,*currentroom);
    if(pickitem!=0)
    {
        if(pickitem->name!="@")
        {
                inventoryAdd(pickitem);
                cout<<"You picked up a/an "<<pickitem->name<<" with a strength of "<<pickitem->strength<<endl;
        }
        else
        {
            delete pickitem;
            cout<<"Pick up what?"<<endl;
        }
    }
    i = checkformove(words,*currentroom);
    if((i)!=5)
    {
        type = 1;
        string out(" ");
        out[0] = i+48;
        if(currentroom->connectedrooms[i]->enterfunction(currentroom->connectedrooms[i])||currentroom->connectedrooms[i]->old)
        {
            currentroom = currentroom->connectedrooms[i];
            if(!currentroom->old)
            {
                cout<<currentroom->newenterstring<<endl<<endl;
            }
            else
            {
                cout<<"You have entered the "<<currentroom->roomname<<endl;
            }
        }
        else
        {
            if(currentroom->connectedrooms[i]->newfailstring!="@")
            {
                cout<<currentroom->connectedrooms[i]->newfailstring<<endl;
            }
            else
            {
                cout<<"Whatever you just tried, it didn't work"<<endl;
            }
        }
        return out;
    }
    doholds(words,*currentroom);
    if((int)s[0]>=48&&(int)s[0]<=52&&currentroom->connectedrooms[(int)s[0]-48]!=0)
    {
        type = 1;
      //  return s;
    }
    if((int)s[0]>=65&&(int)s[0]<=90)
    {
        type = 2;
       // return s;
    }
    delete[] words;
    return s;
}
int main()
{
    init();
    // cout<<numberToString(10)<<endl;
    currentroom = startroom;
    for(int i =0;i<50;i++)
    cout<<endl;
    cout<<startroom->newenterstring<<endl;

    printNavChoices();
    // printItemChoices();
    while(HP>0)
    {
        currentroom->old = true;
        string s = "@";
        type = 0;
        updateEnemies();
        s = getString();
        //cout<<getSecondWord(s)<<endl;

        /*do
        {
            s = "@";
            while((int)s[0]>52||(int)s[0]<48)
            {
                cin>>s;
            }

        }
        while(currentroom->connectedrooms[(int)s[0]-48]==0);*/
        if(type == 2)
        {
            string wordTwo = getSecondWord(s,true);
            switch((int)s[0])
            {
            case 80:
                break;
            case 72:
                if(wordTwo != "@")
                {
                    if(inventoryContains(decaps(wordTwo)))
                    {
                        inventoryAdd(holding);
                        holding = inventoryRemove(decaps(wordTwo));
                        cout<<"You are holding up a/an "<<holding->name<<" with a strength of "<<holding->strength<<endl;
                    }
                    else
                    {
                        cout<<"Hold what?"<<endl;
                    }
                }
                else
                {
                    cout<<"Hold what?"<<endl;
                }

                break;
            case 73:
                printInventory();
                break;
            case 85:
                if(holding!=0)
                cout<<(holding->usefunction()?"":"I'm afraid I can't let you do that, Dave.")<<endl;
                break;
            case 65:
                if(holding!=0)
                {
                    enemy *attacked(getEnemy(wordTwo));
                    if(attacked!=0)
                    {
                        attacked->HP -= holding->strength;
                        if(holding->strength <= 0)
                        {
                            cout<<"You throw the "<<holding->name<<" at "<<attacked->name<<endl;
                            string s = holding->strength<0?("It gained "+numberToString(holding->strength)+" Health"):"Wow.";
                            cout<<s<<endl;
                        }
                        else if(attacked->HP<=0)
                        {
                            attacked->die();
                        }
                    }
                    else
                    {
                        cout<<"You lunge at the air, but hit the ground instead."<<endl;
                    }
                }
                else
                {
                    cout<< "You don't have anything in your hand"<< endl;
                }
            }
        }
        printNavChoices();
    }
    cout<<(winflag?"You Win":"Game Over")<<endl;
    cin>>HP;
    return 0;
}
