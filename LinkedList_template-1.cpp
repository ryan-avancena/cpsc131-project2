#include<iostream>
#include<memory>
#include<vector>
#include<string>

/*
Name: Ryan Avancena
CWID: 885367821
*/

/* NOTES: this looks like a DLL implementation at first glance 
    - in our case, `Tab` is a Node
    - ask about if the cursor should update when i add a new tab
*/

/*
    1. addNewTab() - finished
    2. switchToPrevTab() - finished
    3. switchToNextTab() - finished 
    4. closeCurrentTab() - finished but should debug
    5. bookmarkCurrent() - finished
    6. showBookmarkTab() - finished 
    7. moveCurrentToFirst() - finished but should probably debug
    8. totalMemory() - finished 
    9. deleteTab() - finished but probably should debug
*/

// Write template class Tab here
template<typename T> 
class Tab {
public:
    /* constructor */
    Tab() : url("") , name("") , memory(0), prev(nullptr), next(nullptr) {} ; 
    Tab(std::string _url, std::string _name, T _memory) {
        this->url = _url;
        this->memory = _memory;
        this->name = _name;
        prev = nullptr;
        next = nullptr;  
    }

    void printTab() {
        std::cout << "Current tab is : " << url << " - "<< name << " ... " << memory << "MB/s" << std::endl;
        std::cout << std::endl; 
    }

    /* variables */
    Tab<T>* prev;
    Tab<T>* next; 
    std::string url;
    std::string name;
    T memory;
}; 

// Write template class Browser here
template<typename T>
class Browser {
public:
    /* constructor*/
    Browser() : head(nullptr),tail(nullptr),cursor(nullptr) {};

    /* methods */
    void switchToPrevTab() {
        if (cursor->prev != nullptr) {
            cursor = cursor->prev;
        } else {
            std::cout << "No previous tab." << std::endl << std::endl; 
            return;
        }
        std::cout << "(switchToPrev called) - ";
        cursor->printTab(); 
    }

    void switchToNextTab() {
        if (cursor->next != nullptr) {
            cursor = cursor->next;
        } else {
            std::cout << "No next tab." << std::endl << std::endl; 
            return;
        }
        std::cout << "(switchToNext called) - ";
        cursor->printTab(); 
    }

    void bookmarkCurrent() {
        std::vector<std::pair<std::string,std::string>>::iterator iter = bookmarks.begin(); 

        /* iterate through bookmarks */
        while (iter != bookmarks.end()) {
            auto curr = *iter;
            /* checks if already bookmarked */
            if (cursor->name == curr.first) {
                std::cout << "The bookmark \"" << cursor->url <<"\" is already added!" << std::endl;
                return; 
            }
            iter++;
        }

        std::pair<std::string,std::string> bookmark;
        bookmark.first = cursor->name;
        bookmark.second = cursor->url;
        bookmarks.push_back(bookmark);
    };

    // basically PushBack
    void addNewTab(std::string _url, std::string _name, T _memory) {
        Tab<T> *newTab = new Tab<T>(_url, _name, _memory);

        if (head == nullptr) { // empty 
            head = newTab;
            tail = newTab; 
        } else if (head == tail) { // single node 
            head->next = newTab; 
            newTab->prev = head;
            tail = newTab;
        } else { // more nodes
            tail->next = newTab;
            newTab->prev = tail;
            tail = newTab; 
        }
        cursor = tail;
    }

    void showBookmarkTab() {
        std::cout << std::endl; 
        std::cout << "Bookmarks: " << std::endl; 

        /* iterate through bookmarks */
        std::vector<std::pair<std::string,std::string>>::iterator iter = bookmarks.begin(); 
        while (iter != bookmarks.end()) {
            auto curr = *iter;
            std::cout << curr.first << "(" << curr.second << ")" << std::endl; 
            iter++;
        }
        std::cout << std::endl; 
    }

    /* iterate through tabs and get all memory for each */
    T total_memory() {
        Tab<T> *temp = head;
        T memorySize = 0.00;  
        while (temp != nullptr) {
            memorySize += temp->memory; 
            temp = temp->next;
        }

        return memorySize; 
    }

    void moveCurrentToFirst() {
        if (cursor == nullptr) { // case where there is no tabs
            std::cout << "No tab to move." << std::endl; 
            return;
        } else if (cursor == head) { // case where cursor is at at the front already
            std::cout << "Cursor is already at front." << std::endl; 
            return;
        } else if (cursor == tail) { // case where cursor is at last tab (tail)
            /* resets tail */
            tail = tail->prev; 

            /* resets link for old tail/cursor */
            cursor->prev->next = nullptr; 
            cursor->prev = nullptr;

            /* sets previous to the current tab, sets cursor link to head */
            head->prev = cursor;
            cursor->next = head;

            /* resets head */
            head = head->prev;
            return;
        } else { // case where cursor is in the middle of two tabs
            /* resets links for current tab */
            cursor->prev->next = cursor->next; 
            cursor->next->prev = cursor->prev;

            head->prev = cursor;
            cursor->next = head; 
            head = cursor; 
        }
    }

    void closeCurrentTab() {
        if (cursor == nullptr) { // case where there are no tabs
            std::cout << "No tab to close." << std::endl; 
            return;
        } else if (head == tail) { // case where there is one tab
            delete head;
            head = nullptr;
            tail = nullptr; 
            cursor = nullptr; 
        } else if (cursor == tail) { // case where cursor is at the last tab 
            Tab<T> *temp = tail;
            tail = tail->prev; 
            cursor = tail; 
            temp->prev = nullptr; 
            tail->next = nullptr;
            delete temp; 
            temp = nullptr;
        } else if (cursor == head) { // case where cursor is at first tab
            Tab<T> *temp = head;
            head = head->next;
            cursor = head; 
            head->prev = nullptr;
            temp->next = nullptr; 
            delete temp;
            temp = nullptr; 
        } else { // case where cursor is in the middle of 2 tabs 
            Tab<T> *temp = cursor; 
            cursor->prev->next = cursor->next;
            cursor->next->prev = cursor->prev;
            cursor = temp->next;
            delete temp; 
            temp = nullptr; 
        }
    }

    void deleteTab() {
        /* no tabs in browser */
        if (head == nullptr) {
            std::cout << "No tabs in browser. (deleteTab)" << std::endl; 
            return; 
        }

        float max = 0.00;
        int node = 0;
        Tab<T> *itr = head;
        Tab<T> *del = itr; 
        
        while (itr != nullptr) {
            if (itr->memory > max) {
                max = itr->memory; 
                del = itr; 
            }
            itr = itr->next;
        } 

        std::cout << "The tab using the most memory is : " << del->url << " - "<< del->name << " ... " << del->memory << "MB/s" << std::endl; 
        std::cout << std::endl;
        
        /* CASE W/ SINGLE NODE */
        if (del == tail && head == tail) {
            delete head;
            head = nullptr;
            tail = nullptr; 
            cursor = nullptr; 
            return;
        }

        /* if cursor is pointing to the tab that's taking up the most memory, we move cursor to the tab before or after */
        if (cursor == del) {
            if (cursor->prev != nullptr && cursor == tail) { // if del is last node and has tab in front 
                Tab<T> *temp = tail;
                tail = tail->prev; 
                cursor = tail; 
                temp->prev = nullptr; 
                tail->next = nullptr;
                delete temp; 
                temp = nullptr;
                return;
            } else if (cursor->next != nullptr && cursor == head) { // if del is first node and has tab behind 
                Tab<T> *temp = head;
                head = head->next;
                cursor = head; 
                head->prev = nullptr;
                temp->next = nullptr; 
                delete temp;
                temp = nullptr; 
                return;
            } else if (cursor->prev == nullptr && cursor->next == nullptr) { // normal case where cursor/del is in middle, just do normal deletion but set cursor to node before
                Tab<T> *temp = cursor; 
                cursor->prev->next = cursor->next;
                cursor->next->prev = cursor->prev;
                cursor = temp->prev;
                delete temp; 
                temp = nullptr; 
                return;
            } 
        }

        /* NORMAL DELETION - IF CURSOR ISN'T POINTING TO DELETION TAB*/
        if (del == nullptr) { // case where there are no tabs
            std::cout << "No tab to close. (deleteTab)" << std::endl; 
            return;
        } else if (del == tail) { // case where del is at the last tab 
            tail = tail->prev; 
            cursor = tail; 
            del->prev = nullptr; 
            tail->next = nullptr;
            delete del; 
            del = nullptr;
            return;
        } else if (del == head) { // case where del is at first tab
            head = head->next;
            cursor = head; 
            head->prev = nullptr;
            del->next = nullptr; 
            delete del;
            del = nullptr; 
            return;
        } else { // case where del is in the middle of 2 tabs 
            Tab<T> *temp = del; 
            del->prev->next = del->next;
            del->next->prev = del->prev;
            del->prev = nullptr;
            del->next = nullptr;
            delete del; 
            del = nullptr; 
            return;
        }
    }

    /* variables */
    Tab<T>* head;
    Tab<T>* tail;
    Tab<T>* cursor; 
    std::vector<std::pair<std::string,std::string>> bookmarks;

    void display(){
        auto curr = head;
        std::cout<<"Browser tab list = "<<std::endl;
        while(curr){
            std::cout<<"| "<<curr->name<<"  x|-->";
            curr = curr->next;
        }
        std::cout<<std::endl;
        std::cout<<std::endl;
    }

    /* NOT NEEDED */
    void showCurrent() {
        std::cout << "Current tab is: " << cursor->url << std::endl; 
    }
};

void testingDelete() {
    Browser<double> b1;
    b1.addNewTab("https://www.google.com","Google",23.45);
    b1.addNewTab("https://www.youtube.com","YouTube",56);
    b1.addNewTab("https://www.geeksforgeeks.com","GeeksForGeeks",45.78);
    b1.addNewTab("https://chat.openai.com","ChatGPT",129);
    b1.addNewTab("https://linkedin.com","LinkedIn",410);
    b1.addNewTab("https://github.com","Github",110);
    b1.addNewTab("https://kaggle.com","Kaggle",310);
    b1.display();
    // b1.closeCurrentTab(); 
    // b1.display();
    // b1.switchToPrevTab();
    // b1.switchToPrevTab();
    // b1.closeCurrentTab();
    // b1.display();
    // b1.switchToPrevTab();
    // b1.switchToPrevTab();
    // b1.switchToPrevTab();
    // b1.closeCurrentTab(); 
    // b1.display();
    // b1.switchToNextTab();
    b1.showCurrent();
    b1.deleteTab();
    b1.display();

    b1.showCurrent();
    b1.deleteTab();
    b1.display();

    b1.showCurrent();
    b1.deleteTab();
    b1.display();

    b1.showCurrent();
    b1.deleteTab();
    b1.display();

    b1.showCurrent();
    b1.deleteTab();
    b1.display();
}

int main(){
    Browser<double> b1;
    b1.addNewTab("https://www.google.com","Google",23.45);
    b1.display();
    std::cout<<"Switch to previous tab = "<<std::endl;
    b1.switchToPrevTab();
    std::cout<<"Switch to Next tab = "<<std::endl;
    b1.switchToNextTab();
    b1.addNewTab("https://www.youtube.com","YouTube",56);
    b1.bookmarkCurrent();
    b1.display();
    b1.addNewTab("https://www.geeksforgeeks.com","GeeksForGeeks",45.78);
    b1.bookmarkCurrent();
    b1.addNewTab("https://chat.openai.com","ChatGPT",129);
    b1.addNewTab("https://linkedin.com","LinkedIn",410);
    b1.bookmarkCurrent();
    b1.addNewTab("https://github.com","Github",110);
    b1.addNewTab("https://kaggle.com","Kaggle",310);
    b1.bookmarkCurrent();
    b1.display();
    std::cout<<"Total memory consumption = "<<b1.total_memory()<<"MB"<<std::endl;
    b1.showBookmarkTab();
    b1.moveCurrentToFirst();
    b1.display();
    b1.deleteTab();
    b1.display();
    std::cout<<"Switch to next tab = "<<std::endl;
    b1.switchToNextTab();
    std::cout<<"Switch to previous tab = "<<std::endl;
    b1.switchToPrevTab();
    b1.closeCurrentTab();
    b1.display();
    std::cout<<"Switch to previous tab = "<<std::endl;
    b1.switchToPrevTab();
    b1.closeCurrentTab();
    b1.display();
    b1.showBookmarkTab();
    std::cout<<"Total Memory Consumption = "<<b1.total_memory()<<"MB"<<std::endl;
    b1.deleteTab();
    b1.display();
    b1.addNewTab("https://docs.google.com/","Google Docs",102.34);
    b1.display();
    std::cout<<"Switch to previous tab = "<<std::endl;
    b1.switchToPrevTab();
    std::cout<<"Switch to previous tab = "<<std::endl;
    b1.switchToPrevTab();
    std::cout<<"Switch to previous tab = "<<std::endl;
    b1.switchToPrevTab();
    b1.bookmarkCurrent();
    b1.showBookmarkTab();
    b1.total_memory();
    b1.deleteTab();
    b1.display();
    return 0;
}
