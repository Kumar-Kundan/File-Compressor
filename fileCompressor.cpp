#include<iostream>
#include<unordered_map>
#include<queue>
#include<fstream>
using namespace std;

//creating node for tree
class Node{
    public:
        char ch;
        int freq;
        Node* left;
        Node* right;

        Node(char ch, int freq){
            this->ch=ch;
            this->freq=freq;
            this->left=NULL;
            this->right=NULL;
        }
};

//creating compare function to compare node's frequency in min heap
class Cmp{
    public:
    bool operator()(Node* a, Node* b){
        return a->freq > b->freq;
    }
};

class FileCompressor{
    string fileContent;
    unordered_map<char,int> freqOfChar;
    priority_queue<Node*, vector<Node*>, Cmp> minHeap;

    public:
    //reading file content to a string
    void readFile(string fileName){
        ifstream inputFile(fileName);

        //checking the file is opened or not
        if(!inputFile.is_open()){
            cout<<"Failed to open a input file"<<endl;
            exit(0);
        }
        //reading file content line by line
        string line;
        while(getline(inputFile,line)){
            fileContent+=line+ "\n";
        }
        //closing the file
        inputFile.close();
    }
    
    //counting frequency of characters in map
    void countCharacter(){
        for(char ch:fileContent){
            freqOfChar[ch]++;
        }
    }

    //sorting character a/c to frequency
    //creating node for each entry in map and inserting in min heap
    void insertMinHeap(){
        for(auto mapEntry:freqOfChar){
            Node* newNode=new Node(mapEntry.first,mapEntry.second);
            minHeap.push(newNode);
        }
    }
    void print(priority_queue<Node*, vector<Node*>, Cmp> minHeap){
        while(minHeap.size()>1){
            Node* node=minHeap.top();
            minHeap.pop();

            cout<<node->ch<<" "<<node->freq<<" ";
        }
        cout<<endl;
        
    }
    //creating huffman tree using min heap
    Node* creatTree(){
        while(minHeap.size()>1){
            //taking 2 minimum frequency character
            //and adding them to a new node and insert it in min heap
            Node* left=minHeap.top();
            minHeap.pop();

            Node* right=minHeap.top();
            minHeap.pop();

            int sum=left->freq + right->freq;
            //initializing the new node
            Node *sumNode=new Node('\0',sum);
            sumNode->left=left;
            sumNode->right=right;

            minHeap.push(sumNode);  //insert sum node in heap 
        }
        return minHeap.top();
    }

    //creating encode huffman encoder chart using map
    unordered_map<char,string> encoderChartMap;
    void encoderCode(Node* root, string temp=""){
        //for leaf node
        if(root->left==NULL && root->right==NULL){
            encoderChartMap[root->ch]=temp;  //inserting in map 
            return;
        }

        encoderCode(root->left,temp+"0");
        encoderCode(root->right,temp+"1");
    }

    //creating encode huffman decoder chart using map
    unordered_map<string,char> decoderChartMap;
    void decoderCode(){
        for(auto entry:encoderChartMap){
            decoderChartMap[entry.second]=entry.first;
        }
    }

    //encode function
    void compress(string CfileName){
        readFile(CfileName);  //reading the file

        countCharacter();    //counting the character
        
        insertMinHeap();     //sorting character count

        Node* root=creatTree();    //creating tree

        encoderCode(root);   //huffman coder and decoder
    
        //writing code of each character
        ofstream write(CfileName);

        if(!write.is_open()){
            cout<<"error in opening file"<<endl;
            exit(0);
        }
        for(char ch:fileContent){
            write<<encoderChartMap[ch];
        }
        write.close();
        cout<<"Compressed successfully!!!"<<endl;
    }
    //decode function
    void decompress(string Cfilename,string DfileName){
        decoderCode();   //generating decode chart
        
        fileContent="";
        readFile(Cfilename);

        //writing code of each character
        ofstream write(DfileName);

        if(!write.is_open()){
            cout<<"error in opening file"<<endl;
            exit(0);
        }
        //fetching character for key and writing decodes in file
        string key="";
        for(char ch:fileContent){
            key+=ch;
            if(!decoderChartMap.count(key)){
                continue;
            }
            write<<decoderChartMap[key];
            key="";
        }
        write.close();
        cout<<"Decompressed suuccessfully";
    }
};
//driver code
int main(){
    FileCompressor rar;
    string Cfilename;
    cout<<"Enter file name to compress:"<<endl;
    cin>>Cfilename;
    //calling compress() function
    rar.compress(Cfilename);

    string Dfilename;
    cout<<"Enter filename in which you have to decompress:"<<endl;
    cin>>Dfilename;
    //calling decompress() function
    rar.decompress(Cfilename, Dfilename);
    return 0;
}
