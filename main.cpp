#include <iostream>
#include <string>
#include <conio.h>
#include <cassert>
#include <fstream>
using namespace std;

struct IncidentData{
    int incidentId;
    int locationId;
    string type;
    int urgencyLevel;
    string status;
    IncidentData(int id,int l,string t,int ul):incidentId(id),locationId(l),type(t),urgencyLevel(ul),status("pending"){}
    IncidentData():incidentId(0),locationId(0),type(""),urgencyLevel(5),status("pending"){}
    IncidentData(int id, int l, string t, int ul, string s) :incidentId(id), locationId(l), type(t), urgencyLevel(ul), status(s) {}
};  
class IncidentMinHeap{
    IncidentData *heap;
    int size;
    int lastElement;
    void heapifyUp(int i){
        while(i>0){
            int parent=(i-1)/2;
            if(heap[i].urgencyLevel < heap[parent].urgencyLevel){
                IncidentData temp=heap[i];
                heap[i]=heap[parent];
                heap[parent]=temp;
                i=parent;
                continue;
            }   
            break;
        }
    }
    void heapifyDown(int i){
        while(i<lastElement){
            int small=i;
            int left=i*2+1;
            int right=i*2+2;
            if(left<=lastElement && heap[small].urgencyLevel > heap[left].urgencyLevel )
                small=left;
            if(right<=lastElement && heap[small].urgencyLevel > heap[right].urgencyLevel )
                small=right;
            if(small!=i){
                IncidentData temp=heap[i];
                heap[i]=heap[small];
                heap[small]=temp;
                i=small;
                continue;
            }
            break;
        }
    }
public:
    IncidentMinHeap(int s=50):size(s),lastElement(0){ 
        heap=new IncidentData[size];
    };

    bool insertIncident(IncidentData d){
        if(lastElement>=size){
            cout<<"heap is full can't add more incidents"<<endl;
            return false;
        }
        for(int i=0;i<lastElement;i++){
            if(heap[i].incidentId==d.incidentId){
                cout<<"Incident with this id already exist.error!"<<endl;
                return false;
            }
        }
        heap[lastElement]=d;
        heapifyUp(lastElement);
        lastElement++;
        cout<<"incident added successfully"<<endl;
        return true;
    }
    IncidentData extractMin(){
        if(lastElement<1){
            cout<<"heap is empty.."<<endl;
            return IncidentData();
        }
        IncidentData remover=heap[0];
        heap[0]=heap[--lastElement];
        heapifyDown(0);
        return remover;
    }
    bool isEmpty(){
        return lastElement == 0;
    }
    void printIncidentQueue(){
        cout <<endl<< "----Pending Incidents----"<<endl;
        if (lastElement == 0) {
            cout << "No pending incidents."<<endl;
            return;
        }
        for (int i = 0; i < lastElement; i++) {
            cout << "ID: " << heap[i].incidentId
                 << " | Type: " << heap[i].type
                 << " | Urgency: " << heap[i].urgencyLevel
                 << " | Location: " << heap[i].locationId << endl;
        }
    }
    void saveToStream(ofstream& out) {
        out << lastElement << endl;
        for (int i = 0; i < lastElement; i++) {
            out << heap[i].incidentId << " " << heap[i].locationId << " "
                << heap[i].type << " " << heap[i].urgencyLevel << " " << heap[i].status << endl;
        }
    }

    void loadFromStream(ifstream& in) {
        int count;
        in >> count;
        for (int i = 0; i < count; i++) {
            int id, loc, urg;
            string type, status;
            in >> id >> loc >> type >> urg >> status;
            IncidentData d(id, loc, type, urg, status);
            insertIncident(d);
        }
    }
};

struct QNode {
    int data;
    QNode* next;
    QNode(int d) : data(d), next(nullptr) {}
};
class IntQueue {
    QNode* front;
    QNode* rear;

public:
    IntQueue():front(nullptr),rear(nullptr) {};

    ~IntQueue() {
        while (!isEmpty()) pop();
    }

    bool isEmpty() {
        return front == nullptr;
    }

    void push(int x) {
        QNode* n = new QNode(x);
        if (rear == nullptr) {
            front = rear = n;
            return;
        }
        rear->next = n;
        rear = n;
    }

    void pop() {
        if (isEmpty()) return;
        QNode* temp = front;
        front = front->next;
        if (front == nullptr) rear = nullptr;
        delete temp;
    }

    int frontValue() {
        return front->data;     
    }
};
struct CityNode{
	int dest;
    float weight;
	CityNode*next;
	CityNode(int d,float w):dest(d),weight(w),next(NULL){};
};
struct AdjListNode{
    CityNode *head;
    AdjListNode() : head(nullptr) {}
    void add(int des,int w){
        CityNode*temp=new CityNode(des,w);
        temp->next=head;
        head=temp;
    }
    bool exists(int des) {
        CityNode* temp = head;
        while (temp) {
            if (temp->dest == des)
                return true;
            temp = temp->next;
        }
        return false;
    }
    ~AdjListNode() {
        CityNode* curr = head;
        while (curr) {
            CityNode* next = curr->next;
            delete curr;
            curr = next;
        }
        head = nullptr;
    }
};
class CityMapGraph{
    AdjListNode* list;
    int last;
    bool *have;
    int size;
    
public:
    bool locationExist(int id){
        if(id>=size || !have[id]) return false;
        return true;
    }
    CityMapGraph(int s=50):size(s){
        list = new AdjListNode[size];
        have = new bool[size];
        for (int i = 0; i < size; i++) have[i]=false;
        
    }
    ~CityMapGraph() {
        delete[] list;   
        delete[] have;
    }

    bool addCity(int id){
        if(id >= size){
            cout<<"can't add more locations  "<<endl;
            cout << endl << "Press any key to continue" <<endl;
            _getch();
            return false;
        }
        if(have[id]){
            cout<<"this location already exist "<<endl;
            cout << endl << "Press any key to continue" <<endl;
            _getch();
            return false ;
        }
        have[id]=true;
        cout<<"Location with id "<<id<<" added succesfully"<<endl;
        return true;
    }
    bool addPath(int from,int to,int w){
        if(from == to){
            cout<<"can't add road between same locations."<<endl;
            cout << endl << "Press any key to continue" <<endl;
            _getch();
            return false;
        }
        if(!have[from] || !have[to] ){
            cout<<"the location does't exist"<<endl;
            cout << endl << "Press any key to continue" <<endl;
            _getch();
            return false;
        }
        if (list[from].exists(to)) {
            cout<<"Road already exists between "<<from<<" and "<<to<<endl;
            cout << endl << "Press any key to continue" <<endl;
            _getch();
            return false;
        }
        list[from].add(to,w);
        // list[to].add(from,w);
        cout<<"Road added from location "<<from<<" to "<<to<<endl;
        return true;
    }

    int bfsDistance(int start,int end){
        if(!have[start]||!have[end]){
            return -1;
        }
        bool *visited=new bool [size];
        int *distance=new int [size];
        for(int i=0;i<size;i++){
            visited[i]=false;
            distance[i]=-1;
        } 
        IntQueue q;
        q.push(start);
        visited[start]=true;
        distance[start]=0;
        while(!q.isEmpty()){
            int n=q.frontValue();
            q.pop();
            if(n==end){
                int ret=distance[end];
                delete[] visited;
                delete[] distance;
                return ret;
            }
            CityNode*node=list[n].head;
            while(node){
                int c=node->dest;
                if(!visited[c]){
                    visited[c]=true;
                    q.push(c);
                    distance[c]=distance[n]+1;
                }
                node=node->next;
            }

        }
        delete[] visited;
        delete[] distance;
        return -1;
    }
    void display(){
        cout <<endl<< "----City Map----"<<endl;
        for (int i = 0; i < size; i++){
            if (have[i]) {
                cout << "Location " << i << " -> ";
                CityNode *edge = list[i].head;
                if (!edge) cout << "(no roads)";
                while (edge != nullptr)
                {
                    cout << edge->dest << "(w:" << edge->weight << ") ";
                    edge = edge->next;
                }
                cout << endl;
            }
        }
    }
    // --- SAVE / LOAD ---
    void saveToStream(ofstream& out) {
        // Save active nodes
        out << size << endl;
        for (int i = 0; i < size; i++) {
            if (have[i]) out << i << " ";
        }
        out << -1 << endl; // Node sentinel

        // Save edges
        for (int i = 0; i < size; i++) {
            if (have[i]) {
                CityNode* edge = list[i].head;
                while (edge != nullptr) {
                    out << i << " " << edge->dest << " " << edge->weight << endl;
                    edge = edge->next;
                }
            }
        }
        out << -1 << " " << -1 << " " << -1 << endl; // Edge sentinel
    }

    void loadFromStream(ifstream& in) {
        int cap;
        in >> cap; // Skip capacity, we assume 50

        int nodeId;
        // Read nodes until sentinel -1
        while (in >> nodeId && nodeId != -1) {
            addCity(nodeId);
        }

        int u, v;
        float w;
        // Read edges until sentinel -1
        while (in >> u >> v >> w && u != -1) {
            addPath(u, v, (int)w);
        }
    }
};

struct VehicleData{
    int vehicleId;
    int locationId;
    string type;
    string status;
    VehicleData(int id,int l,string t):vehicleId(id),locationId(l),type(t),status("available"){}
    VehicleData(int id, int l, string t, string s) :vehicleId(id), locationId(l), type(t), status(s) {}
    VehicleData():vehicleId(0),locationId(0),type(""),status("available"){}
};
struct VehicleDataNode{
    VehicleData vehicle;
    VehicleDataNode*next;
    VehicleDataNode(VehicleData v):vehicle(v),next(nullptr){}
};
class VehicleHashTable{
    VehicleDataNode**table;
    int size;
    int hash(int key){
        return key%size;
    }
public:
    VehicleDataNode** getTable() { return table; }
    VehicleHashTable():size(10){
        table=new VehicleDataNode*[size];
        for(int i=0;i<size;i++){
            table[i]=nullptr;   
        }
    }
    void display(){
        cout <<endl<< "-----All Vehicles----"<<endl;
        bool found=false;
        for (int i = 0; i < size; i++) {
            VehicleDataNode *temp = table[i];
            while (temp) {
                found=true;
                cout << "ID: " << temp->vehicle.vehicleId
                     << " | Type: " << temp->vehicle.type
                     << " | Status: " << temp->vehicle.status
                     << " | Location: " << temp->vehicle.locationId <<endl;
                temp = temp->next;
            }
        }
        if(!found) cout<<"No vehicles..."<<endl;
    }
    bool remove(int id){
        int idx=hash(id);
        VehicleDataNode*curr=table[idx],*prev=nullptr;
        while(curr){
            if(curr->vehicle.vehicleId==id){
                if(curr->vehicle.status!="available"){
                    cout<<"Cannot remove vehicle right now because it is enRoute or under maintainance."<<endl;
                    return false;
                }
                if(!prev)
                    table[idx]=curr->next;
                else
                    prev->next=curr->next;
                delete curr;
                cout<<"Vehicle removed successfully"<<endl;
                return true;
            }
            prev=curr;
            curr=curr->next;
        }
        cout<<"Vehilce not found "<<endl;
        return false;
    }
    bool insert(VehicleData v){
        int idx=hash(v.vehicleId);
        VehicleDataNode *temp=table[idx];
        while(temp){
            if(temp->vehicle.vehicleId==v.vehicleId){
                cout<<"Vehicle with this Id already exist.error!"<<endl;
                return false;
            }
            temp=temp->next;
        }
        VehicleDataNode *node=new VehicleDataNode(v);
        node->next=table[idx];
        table[idx]=node;
        return true;
    }
    VehicleData* findClosestAvailable(string type, int locationID, CityMapGraph &map,int maxHops) {
        VehicleData *closestVehicle = nullptr;
        int minDistance = 999999;

        for (int i = 0; i < size; i++) {
            VehicleDataNode *current = table[i];
            while (current) {
                if (current->vehicle.type == type && current->vehicle.status == "available") {
                    int dist = map.bfsDistance(current->vehicle.locationId,locationID);
                    if (dist != -1 && dist <= maxHops && dist < minDistance){
                        minDistance = dist;
                        closestVehicle = &(current->vehicle);
                    }
                }
                current = current->next;
            }
        }

        return closestVehicle;
    }

    // VehicleData* search(int id){
    //     int idx=hash(id);
    //     VehicleDataNode*node=table[idx];
    //     while(node){
    //         if(node->vehicle.vehicleId==id){
    //             return &node->vehicle;
    //         }
    //         node=node->next;
    //     }
    //     return nullptr;
    // }

    // --- SAVE / LOAD ---
    void saveToStream(ofstream& out) {
        int count = 0;
        for (int i = 0; i < size; i++) {
            VehicleDataNode* temp = table[i];
            while (temp) { count++; temp = temp->next; }
        }
        out << count << endl;
        for (int i = 0; i < size; i++) {
            VehicleDataNode* temp = table[i];
            while (temp) {
                out << temp->vehicle.vehicleId << " " << temp->vehicle.locationId << " "
                    << temp->vehicle.type << " " << temp->vehicle.status << endl;
                temp = temp->next;
            }
        }
    }

    void loadFromStream(ifstream& in) {
        int count;
        in >> count;
        for (int i = 0; i < count; i++) {
            int id, loc;
            string type, status;
            in >> id >> loc >> type >> status;
            VehicleData v(id, loc, type, status);
            insert(v);
        }
    }
};

struct HistoricalNode{
    IncidentData incidentdata;
    HistoricalNode*left,*right;
    int height;
    HistoricalNode(IncidentData inc) : incidentdata(inc), left(nullptr), right(nullptr), height(1) {}
};
class HistoricalNodeAVL{
    HistoricalNode*root;
    int getHeight(HistoricalNode *node){
        return node ? node->height : 0;
    }
    int getBalance(HistoricalNode *node){
        return node ? getHeight(node->left) - getHeight(node->right) : 0;
    }
    void updateHeight(HistoricalNode *node){
        if (node){
            int hl = getHeight(node->left);
            int hr = getHeight(node->right);
            node->height = 1 + (hl > hr ? hl : hr);
        }
    }
    HistoricalNode*rotateRight(HistoricalNode*node){
        HistoricalNode*subtree=node->left;
        node->left=subtree->right;
        subtree->right=node;
        updateHeight(node);
        updateHeight(subtree);
        return subtree;
    }
    HistoricalNode*rotateLeft(HistoricalNode*node){
        HistoricalNode*subtree=node->right;
        node->right=subtree->left;
        subtree->left=node;
        updateHeight(node);
        updateHeight(subtree);
        return subtree;
    }
    HistoricalNode *insert(HistoricalNode *node, IncidentData incident){
        if (node == nullptr)
            return new HistoricalNode(incident);
        if (incident.incidentId < node->incidentdata.incidentId)
            node->left = insert(node->left, incident);
        else if (incident.incidentId > node->incidentdata.incidentId)
            node->right = insert(node->right, incident);
        else
            return node; 
        updateHeight(node);
        int balance = getBalance(node);
        if (balance > 1 && incident.incidentId < node->left->incidentdata.incidentId) return rotateRight(node);
        
        if (balance < -1 && incident.incidentId > node->right->incidentdata.incidentId) return rotateLeft(node);

        if (balance > 1 && incident.incidentId > node->left->incidentdata.incidentId) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }

        if (balance < -1 && incident.incidentId < node->right->incidentdata.incidentId){
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }
        return node;
    }
    void inorder(HistoricalNode *node){
        if (node) {
            inorder(node->left);
            cout << "ID: " << node->incidentdata.incidentId
                 << " | Type: " << node->incidentdata.type
                 << " | Urgency: " << node->incidentdata.urgencyLevel
                 << " | Status: " << node->incidentdata.status << endl;
            inorder(node->right);
        }
    }
    HistoricalNode *searchNode(HistoricalNode *node, int id){
        if (!node || node->incidentdata.incidentId == id)
            return node;
        if (id < node->incidentdata.incidentId )
            return searchNode(node->left, id);
        else
            return searchNode(node->right, id);
    }
    void preOrderSave(HistoricalNode* node, ofstream& out) {
        if (node) {
            out << node->incidentdata.incidentId << " "
                << node->incidentdata.locationId << " "
                << node->incidentdata.type << " "
                << node->incidentdata.urgencyLevel << " "
                << node->incidentdata.status << endl;
            preOrderSave(node->left, out);
            preOrderSave(node->right, out);
        }
    }
public:
    HistoricalNodeAVL() : root(nullptr) {}
    void generateReport(){
        cout<<endl << "----Historical Incident Report (In-Order) ---"<<endl;
        if (!root){
            cout << "No records"<<endl;
            cout << endl << "Press any key to continue" <<endl;
            _getch();
            return;
        }
        inorder(root);
        cout << endl << "Press any key to continue" <<endl;
        _getch();
    }
    void insertIncident(IncidentData incident){
        root = insert(root, incident);
    }
    IncidentData* searchIncident(int id){
        HistoricalNode *node = searchNode(root, id);
        return node ? &(node->incidentdata) : nullptr;
    }
    // --- SAVE / LOAD ---
    void saveToStream(ofstream& out){
        preOrderSave(root, out);
    }

    void loadFromStream(ifstream& in){
        int id, loc, urg;
        string type, status;
        while (in >> id >> loc >> type >> urg >> status) {
            IncidentData d(id, loc, type, urg, status);
            insertIncident(d);
        }
    }
};

struct QueueNode{
    VehicleData*vehicle;
    QueueNode *next;
    QueueNode(VehicleData * v) :vehicle(v), next(nullptr) {}
};
class TaskQueue{
private:
    QueueNode *front;
    QueueNode *rear;

public:
    TaskQueue() : front(nullptr), rear(nullptr) {}

    ~TaskQueue() {
        while (front != nullptr)
        {
            QueueNode *temp = front;
            front = front->next;
            delete temp;
        }
    }

    void enqueue(VehicleData*v) {
        QueueNode *newNode = new QueueNode(v);
        if (!rear)
            front = rear = newNode;
        else {
            rear->next = newNode;
            rear = newNode;
        }
        cout << "Vehicle "<<newNode->vehicle->vehicleId <<" is under maintainance . " << endl;
    }
    
    void dequeue() {
        if (!front ) {
            cout << "queue is empty"<<endl;
            cout << endl << "Press any key to continue" <<endl;
            _getch();
            return ;
        }
        QueueNode *temp = front;
        front = front->next;
        temp->vehicle->status = "available";
        cout << "Maintainace of Vehilce  "<<temp->vehicle->vehicleId<<" completed " << endl;
        if (!front){
            rear = nullptr;
        }
        delete temp;
        cout << endl << "Press any key to continue" <<endl;
        _getch();
    }
    void display() {
        cout <<endl<<"---- Maintenance Task Queue ----"<<endl;
        if (front == nullptr) {
            cout << "No pending maintenance tasks."<<endl;
            cout << endl << "Press any key to continue" <<endl;
            _getch();
            return;
        }
        QueueNode *node = front;
        int count = 1;
        while (node) {
            cout << count++ << ". vehicle "<<node->vehicle->vehicleId << endl;
            node = node->next;
        }
        cout << endl << "Press any key to continue" <<endl;
        _getch();
    }
    void saveToStream(ofstream& out) {
        QueueNode* node = front;

        // Count nodes
        int count = 0;
        while (node) {
            count++;
            node = node->next;
        }

        out << count << endl;

        node = front;
        while (node) {
            out << node->vehicle->vehicleId << endl;
            node = node->next;
        }
    }

    void loadFromStream(ifstream& in, VehicleHashTable& vehicleHash) {
        int count;
        in >> count;

        for (int i = 0; i < count; i++) {
            int vid;
            in >> vid;

            // find correct pointer through hash table
            VehicleData* v = nullptr;

            for (int b = 0; b < 50; b++) {
                VehicleDataNode* temp = vehicleHash.getTable()[b];
                while (temp) {
                    if (temp->vehicle.vehicleId == vid) {
                        v = &(temp->vehicle);
                        break;
                    }
                    temp = temp->next;
                }
                if (v) break;
            }

            if (v) enqueue(v);
        }
    }

};

struct StackNode {
    string command;
    StackNode *next;
    StackNode(string s) : command(s), next(nullptr) {}
};
class CommandStack{
private:
    StackNode *top;
public:
    CommandStack() : top(nullptr) {}
    ~CommandStack() {
        while (top != nullptr) {
            StackNode *temp = top;
            top = top->next;
            delete temp;
        }
    }

    void push(string s){
        StackNode *node = new StackNode(s);
        node->next = top;
        top = node;
    }

    void pop(){
        if (!top ) {
            cout << "stack is empty"<<endl;
            return ;
        }
        StackNode *temp = top;
        top = top->next;
        delete temp;
    }

    string peek() {
        if (!top ) return "stack is empty";
        return top->command;
    }

    void display(){
        if (!top ) {
            cout << "stack is empty"<<endl;
            cout << endl << "Press any key to continue" <<endl;
        _getch();
            return ;
        }
        cout <<endl<<" Command History---"<<endl;
        StackNode *node = top;
        int count = 1;
        while (node)
        {
            cout << count++ << ". " << node->command <<endl;
            node = node->next;
        }
        cout << endl << "Press any key to continue" <<endl;
        _getch();
    }
    void saveToStream(ofstream& out) {
        // Count nodes
        int count = 0;
        StackNode* node = top;
        while (node) {
            count++;
            node = node->next;
        }

        out << count << endl;

        // Reverse the stack into a temporary list (bottom will become head)
        StackNode* rev = nullptr;
        node = top;
        while (node) {
            StackNode* n = new StackNode(node->command);
            n->next = rev;
            rev = n;
            node = node->next;
        }

        // Save in correct order (bottom→top)
        node = rev;
        while (node) {
            out << node->command << endl;
            node = node->next;
        }

        // Free temp reversed list
        node = rev;
        while (node) {
            StackNode* del = node;
            node = node->next;
            delete del;
        }
    }
    void loadFromStream(ifstream& in) {
        int count;
        in >> count;
        in.ignore(); // skip newline

        // Build a temporary list in input order (bottom→top)
        StackNode* rev = nullptr;
        StackNode* revTail = nullptr;

        for (int i = 0; i < count; i++) {
            string s;
            getline(in, s);

            StackNode* n = new StackNode(s);

            if (rev == nullptr) {
                rev = n;
                revTail = n;
            } else {
                revTail->next = n;
                revTail = n;
            }
        }

        // Now push onto actual stack in correct order
        StackNode* node = rev;
        while (node) {
            push(node->command);
            node = node->next;
        }

        // Free temp list
        node = rev;
        while (node) {
            StackNode* del = node;
            node = node->next;
            delete del;
        }
    }
};

struct DispatchedNode {
    IncidentData incident;
    VehicleData* vehicle;
    DispatchedNode* next;
    DispatchedNode(IncidentData d,VehicleData*v) : incident(d),vehicle(v) , next(nullptr) {}
};
class DispatchedList {
public:
    DispatchedNode* head;

    DispatchedList():head(nullptr) {}

    void addDispatched(IncidentData d,VehicleData*v) {
        DispatchedNode* temp = new DispatchedNode(d,v);
        temp->next = head;
        head = temp;
    }   
    bool isEmpty(){
        return head == nullptr;
    }
    DispatchedNode* findDispatched(int id) {
        DispatchedNode* curr = head;
        while (curr) {
            if (curr->incident.incidentId == id)
                return curr;
            curr = curr->next;
        }
        return nullptr;
    }

    bool removeDispatched(int id) {
        if (isEmpty()) return false;

        if (head->incident.incidentId == id) {
            DispatchedNode* del = head;
            head = head->next;
            delete del;
            return true;
        }
        DispatchedNode* curr = head;
        while (curr->next != nullptr) {
            if (curr->next->incident.incidentId == id) {
                DispatchedNode* del = curr->next;
                curr->next = curr->next->next;
                delete del;
                return true;
            }
            curr = curr->next;
        }
        return false;
    }

    void displayAll() {
        cout << endl << "----Dispatched Incidents----" << endl;
        if (!head) {
            cout << "No dispatched incidents"<<endl;
            return;
        }
        DispatchedNode* curr = head;
        while (curr) {
            cout << "ID: " << curr->incident.incidentId
                 << " | Type: " << curr->incident.type
                 << " | Urgency: " << curr->incident.urgencyLevel
                 << " | Location: " << curr->incident.locationId
                 << " | Status: " << curr->incident.status << endl;
            curr = curr->next;
        }
    }
        // --- SAVE / LOAD ---
    void saveToStream(ofstream& out) {
        DispatchedNode* curr = head;

        // Count how many dispatched nodes
        int count = 0;
        while (curr) {
            count++;
            curr = curr->next;
        }

        out << count << endl;

        curr = head;
        while (curr) {
            out << curr->incident.incidentId << " "
                << curr->incident.locationId << " "
                << curr->incident.type << " "
                << curr->incident.urgencyLevel << " "
                << curr->incident.status << " "
                << curr->vehicle->vehicleId      // store just ID
                << endl;

            curr = curr->next;
        }
    }

    void loadFromStream(ifstream& in, VehicleHashTable& vehicleHash) {
        int count;
        in >> count;

        for (int i = 0; i < count; i++) {
            int id, loc, urg, vid;
            string type, status;

            in >> id >> loc >> type >> urg >> status >> vid;

            // Reconstruct incident
            IncidentData inc(id, loc, type, urg, status);

            // Fetch vehicle by ID
            VehicleData* v = nullptr;

            // scan hash table to find pointer
            for (int b = 0; b < 50; b++) {
                VehicleDataNode* temp = vehicleHash.getTable()[b];
                while (temp) {
                    if (temp->vehicle.vehicleId == vid) {
                        v = &(temp->vehicle);
                        break;
                    }
                    temp = temp->next;
                }
                if (v) break;
            }

            // Only add if vehicle found (safety)
            if (v) {
                addDispatched(inc, v);
            }
        }
    }

};

class ERDS{
    IncidentMinHeap incidentheap;
    VehicleHashTable vehicleHash;
    CityMapGraph cityMap;
    HistoricalNodeAVL historicalAVL;
    TaskQueue maintainanceQueue;
    CommandStack CommandStack;
    DispatchedList dispatchedList; 
    int nextIncidentId;
    int nextVehicleId;
    int nextCityId;
    int maxHops;
    
    bool isValidIncidentType(string &lowerType) {
        // string lowerType = type;
        for (int i = 0; i < lowerType.length(); i++) {
            if (lowerType[i] >= 'A' && lowerType[i] <= 'Z') {
                lowerType[i] = lowerType[i] + 32;
            }
        }
        return (lowerType == "medical" || lowerType == "fire" || lowerType == "crime");
    }
    bool isValidVehicleType(string &lowerType) {
        // string lowerType = type;
        for (int i = 0; i < lowerType.length(); i++) {
            if (lowerType[i] >= 'A' && lowerType[i] <= 'Z') {
                lowerType[i] = lowerType[i] + 32;
            }
        }
        return (lowerType == "ambulance" || lowerType == "fire" || lowerType == "police");
    }
    void addNewIncident(){
        cout <<endl<< "____Add incident____"<<endl;
        int l,ul;
        string t;
        cout<<"Enter location id of incident :";cin>>l;assert(!cin.fail());
        if(!cityMap.locationExist(l)){
            cout<<"this location id does not exist in map ";
            cout << endl << "Press any key to continue" <<endl;
            _getch();
            return;
        }
        cout<<"Enter urgency level (1-5) : ";cin>>ul;assert(!cin.fail());
        if(ul<1 || ul>5 ) {
            cout<<"invalid Urgency level.logging the incident is cancelled";
            cout << endl << "Press any key to continue" <<endl;
            _getch();
            return;
        }
        cout << "Enter incident type (medical/fire/crime): ";
        cin >> t;assert(!cin.fail());
        if (!isValidIncidentType(t)) {
            cout << "invalid incident type,it must be medical,fire or crime." << endl;
            cout << "logging the incident is cancelled." << endl;
            cout << endl << "Press any key to continue" <<endl;
            _getch();
            return;
        }
        IncidentData incident(nextIncidentId,l,t,ul);
        if(incidentheap.insertIncident(incident))
            CommandStack.push("Logged incident ID " + to_string(incident.incidentId));
        cout << endl << "Press any key to continue" <<endl;
        _getch();
    }
    void dispatchNextIncident() {
        if (incidentheap.isEmpty()){
            cout << "Heap is empty. No pending incidents."<<endl;
            cout << endl << "Press any key to continue" <<endl;
            _getch();
            return;
        }
        IncidentData incident = incidentheap.extractMin();
        cout <<endl<< "____Dispatching Incident " << incident.incidentId << " ____"<<endl;
        cout << "Type: " << incident.type << " | Urgency: " << incident.urgencyLevel
        << " | Location: " << incident.locationId << "\n";
        
        string vehicleType;
        if (incident.type == "medical")
        vehicleType = "ambulance";
        else if (incident.type == "fire")
        vehicleType = "fire";
        else if (incident.type == "crime")
        vehicleType = "police";
        
        VehicleData *closestVehicle = vehicleHash.findClosestAvailable(vehicleType, incident.locationId, cityMap,maxHops);

        if (closestVehicle == nullptr){
            cout << "No available vehicles of type " << vehicleType << " found within " << maxHops << " hops." << endl;
            incidentheap.insertIncident(incident); 
            cout << endl << "Press any key to continue" <<endl;
            _getch();
            return;
        }
        int actualHops = cityMap.bfsDistance(closestVehicle->locationId, incident.locationId);
        
        cout << "Closest Vehicle Found: ID " << closestVehicle->vehicleId 
         << " at Location " << closestVehicle->locationId 
         << " (" << actualHops << " hops away)" << endl;

        closestVehicle->status = "enRoute";
        closestVehicle->locationId = incident.locationId;
        incident.status = "dispatched";

        CommandStack.push("Dispatched Vehicle " + to_string(closestVehicle->vehicleId) +" for Incident " + to_string(incident.incidentId));

        cout << "Incident " << incident.incidentId << " dispatched successfully "<<endl;
        cout<<"Vehicle "<< closestVehicle->vehicleId << " is Enroute."<<endl;

        dispatchedList.addDispatched(incident,closestVehicle);
        cout << endl << "Press any key to continue" <<endl;
        _getch();
    }
    void resolveIncident() {
        if(dispatchedList.isEmpty()){
            cout << "No Incidents are dispatched to be resolved "<<endl;
            cout << endl << "Press any key to continue" <<endl;
            _getch();     
            return ;
        }
        cout << endl<<"____Resolving Incident____"<<endl;
        int id;
        cout << "Enter Dispatched Incident ID to resolve: ";
        cin >> id;assert(!cin.fail());
        

        DispatchedNode* dispatched = dispatchedList.findDispatched(id);
        
        if (!dispatched) {
            cout << "Incident " << id << " not found in dispatched list.\n";
            cout << endl << "Press any key to continue" <<endl;
            _getch();
            return;
        }

        cout << "Resolving Incident " << dispatched->incident.incidentId  << endl;
        cout << "Type: " << dispatched->incident.type 
        << " | Urgency: " <<  dispatched->incident.urgencyLevel
             << " | Location: " <<  dispatched->incident.locationId << endl;
             
        IncidentData resolved;
        resolved.incidentId =  dispatched->incident.incidentId;
        resolved.locationId =  dispatched->incident.locationId;
        resolved.type =  dispatched->incident.type;
        resolved.urgencyLevel =  dispatched->incident.urgencyLevel;
        resolved.status = "Closed";
        dispatched->vehicle->status="maintainance";
        
        historicalAVL.insertIncident(resolved);
        
        dispatchedList.removeDispatched(id);
        maintainanceQueue.enqueue(dispatched->vehicle);
        
        CommandStack.push("Resolved Incident " + to_string(id));
        cout << "Incident " << id << " resolved and moved to history" << endl;
        cout << endl << "Press any key to continue" <<endl;
        _getch();
    }
    void addVehicle(){
        int  l;
        string t;
        cout <<endl<< "____Add Vehicle____"<<endl;
        cout << "type (ambulance/fire/police): ";
        cin >> t;assert(!cin.fail());
        if (!isValidVehicleType(t)) {
            cout << "invalid incident type,it must be ambulance/fire/police" << endl;
            cout << "adding the vehicle is cancelled." << endl;
            cout << endl << "Press any key to continue" <<endl;
            _getch();
            return;
        }
        cout << "Current Location id: ";
        cin >> l;assert(!cin.fail());
        if(!cityMap.locationExist(l)){
            cout<<"this location id does not exist in map ";
            cout << endl << "Press any key to continue" <<endl;
            _getch();
            return;
        }
        VehicleData v(nextVehicleId,l,t);
        if(vehicleHash.insert(v))
           CommandStack.push("Added Vehicle " + to_string(v.vehicleId));
        cout << endl << "Press any key to continue" <<endl;
        _getch();
    }
    void removeVehicle() {
        int id;
        cout <<endl<< "____Remove Vehicle ____"<<endl;
        cout << "Vehicle ID: ";
        cin >> id;assert(!cin.fail());
        if (vehicleHash.remove(id))
            CommandStack.push("Removed Vehicle " + to_string(id));
        cout << endl << "Press any key to continue" <<endl;
        _getch();
    }
    bool addLocation() {
        if(cityMap.addCity(nextCityId)){
            CommandStack.push("Added Location " + to_string(nextCityId));
            return true;
        }
        return false;
    }
    void viewMaxhops(){
        cout<<"Max hops is : "<<maxHops<<endl;
        cout << endl << "Press any key to continue" << endl;
        _getch();
    }
    void setMaxHops() {
        int oldMaxHops = maxHops; 
        cout << "Current max hops limit: " << maxHops << endl;
        cout << "Enter new max hops limit: ";
        cin >> maxHops;
        assert(!cin.fail());
        CommandStack.push("Changed max hops from " + to_string(oldMaxHops) + " to " + to_string(maxHops));
        
        cout << endl << "Press any key to continue" << endl;
        _getch();
    }
    void addRoad(){
        int a, b, w;
        cout <<endl<< "____Add Road  ____"<<endl;
        cout << "From: ";
        cin >> a;assert(!cin.fail());
        cout << "To: ";
        cin >> b;assert(!cin.fail()); 
        cout << "Weight: ";
        cin >> w;assert(!cin.fail());
        if(cityMap.addPath(a,b,w))
        CommandStack.push("added road from " + to_string(a) + " to " + to_string(b));
    }
    // --- SYSTEM SAVE FUNCTION ---
    void saveSystem() {
        cout << "Saving Data..." << endl;

        ofstream incidentFile("incidents.txt");
        if (incidentFile.is_open()) {
            incidentheap.saveToStream(incidentFile);
            incidentFile.close();
        }

        ofstream vehicleFile("vehicles.txt");
        if (vehicleFile.is_open()) {
            vehicleHash.saveToStream(vehicleFile);
            vehicleFile.close();
        }

        ofstream historyFile("history.txt");
        if (historyFile.is_open()) {
            historicalAVL.saveToStream(historyFile);
            historyFile.close();
        }

        ofstream graphFile("graph.txt");
        if (graphFile.is_open()) {
            cityMap.saveToStream(graphFile);
            graphFile.close();
        }

        ofstream configFile("config.txt");
        if (configFile.is_open()) {
            configFile << nextIncidentId << " " << nextVehicleId << " " << nextCityId << endl;
            configFile.close();
        }

        ofstream dispatchedFile("dispatched.txt");
        if (dispatchedFile.is_open()) {
            dispatchedList.saveToStream(dispatchedFile);
            dispatchedFile.close();
        }

        ofstream taskFile("tasks.txt");
        if (taskFile.is_open()) {
            maintainanceQueue.saveToStream(taskFile);
            taskFile.close();
        }

        ofstream cmdFile("commands.txt");
        if (cmdFile.is_open()) {
            CommandStack.saveToStream(cmdFile);
            cmdFile.close();
        }


        cout << "System state saved successfully." << endl;
    }

    // --- SYSTEM LOAD FUNCTION ---
    bool loadSystem() {
        // 1. Check if config file exists (implies system was saved)
        ifstream configFile("config.txt");
        if (!configFile.is_open()) {
            cout << "No saved data found." << endl;
            cout << endl << "Press any key to continue" << endl;
            _getch();
            return false; // No save data found
        }
        cout << "System data loaded successfully." << endl;
        cout << endl << "Press any key to continue" << endl;
        _getch();

        // 2. Restore ID counters
        configFile >> nextIncidentId >> nextVehicleId >> nextCityId;
        configFile.close();
        cout << "Previous session found. Restoring..." << endl;

        // 3. Restore Graph
        ifstream graphFile("graph.txt");
        if (graphFile.is_open()) {
            cityMap.loadFromStream(graphFile);
            graphFile.close();
        }

        // 4. Restore Vehicles
        ifstream vehicleFile("vehicles.txt");
        if (vehicleFile.is_open()) {
            vehicleHash.loadFromStream(vehicleFile);
            vehicleFile.close();
        }

        // 5. Restore Pending Incidents
        ifstream incidentFile("incidents.txt");
        if (incidentFile.is_open()) {
            incidentheap.loadFromStream(incidentFile);
            incidentFile.close();
        }

        // 6. Restore Historical Data
        ifstream historyFile("history.txt");
        if (historyFile.is_open()) {
            historicalAVL.loadFromStream(historyFile);
            historyFile.close();
        }

        ifstream dispatchedFile("dispatched.txt");
        if (dispatchedFile.is_open()) {
            dispatchedList.loadFromStream(dispatchedFile, vehicleHash);
            dispatchedFile.close();
        }
        
        ifstream taskFile("tasks.txt");
        if (taskFile.is_open()) {
            maintainanceQueue.loadFromStream(taskFile, vehicleHash);
            taskFile.close();
        }

        ifstream cmdFile("commands.txt");
        if (cmdFile.is_open()) {
            CommandStack.loadFromStream(cmdFile);
            cmdFile.close();
        }

        return true;
    }
    

public:    
    ERDS():nextIncidentId(101), nextVehicleId(201),nextCityId(0),maxHops(4){
        cout << "_____ Welcome to ERDS _____" << endl;

        char choice;
        cout << "Do you want to load the previous session? (y/n): ";
        cin >> choice;assert(!cin.fail());

        bool loaded = false;
        if (choice == 'y' || choice == 'Y') {
            loaded = loadSystem();
            if (!loaded) {
                cout << "No save files found. Starting a new session." << endl;
            }
        }
        if (!loaded) {
            int n;
            cout << "Starting new system setup..." << endl;
            cout << "Enter the number of locations to create: ";
            cin >> n; assert(!cin.fail());
            for (int i = 0; i < n; i++) {
                if (addLocation()) nextCityId++;
                else break;
            }
            if(n<=0) return;
            char add;
            do {
                addRoad();
                cout << "Do you want to add another road? (y/n): ";
                cin >> add; assert(!cin.fail());
            } while (add == 'y' || add == 'Y');
        }

    }
    void mainMenu(){
        int choice;
        do {
            system("cls");
            cityMap.display();
            incidentheap.printIncidentQueue();
            vehicleHash.display();
            dispatchedList.displayAll();
            cout << "\n========== ERDS MAIN MENU ==========\n";
            cout << "1.  Log New Incident\n";
            cout << "2.  Dispatch Next Incident\n";
            cout << "3.  resolve Incident\n";
            cout << "4.  Add Vehicle\n";
            cout << "5.  remove Vehicle\n";
            cout << "6.  Add Location\n";
            cout << "7.  add Road\n";
            cout << "8.  view maintenance Task\n";
            cout << "9.  Process Maintenance Task\n";
            cout << "10. View Command History\n";
            cout << "11. View Historical Report\n";
            cout << "12. view max hops limit\n";
            cout << "13. Set max hops limit\n";
            cout << "14. Save & Exit\n";
            cout << "====================================\n";
            cout << "Enter choice: ";
            cin >> choice; assert(!cin.fail());
            switch (choice){
            case 1:
                addNewIncident();
                nextIncidentId++;
                break;
            case 2:
                dispatchNextIncident();
                break;
            case 3:
                resolveIncident();
                break;
            case 4:
                addVehicle();
                nextVehicleId++;
                break;
            case 5:
                removeVehicle();
                break;
            case 6:
                addLocation();
                nextCityId++;
                break;
            case 7:
                addRoad();
                break;
            case 8:
                maintainanceQueue.display();
                break;
            case 9: 
                maintainanceQueue.dequeue();
                break;
            case 10:
                CommandStack.display();
                break;
            case 11:
                historicalAVL.generateReport();
                break;
            case 12:
                viewMaxhops();
                break;
            case 13:
                setMaxHops();
                break;
            case 14:
                saveSystem(); 
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid option.\n";
                cout << endl << "Press any key to continue" <<endl;
                _getch();
                break;
            }
        }while (choice != 14);
    }

};

int main() {
    system("cls");
    ERDS sys;
    sys.mainMenu();
    return 0;
}