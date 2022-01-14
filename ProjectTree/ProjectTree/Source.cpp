#include <iostream>
#include <iomanip>
#include <sstream>
#include <queue>
#include <string>

/*Here is the node class where we initialize only the coolest nodes. We ensure we
have a height variable for easy balancing later as well as our pointers */
class TreeNode {
public:
    std::string NAME, GatorID;
    int height = 1;
    TreeNode* left;
    TreeNode* right;
    TreeNode() : GatorID(""), NAME("empty"), left(nullptr), right(nullptr) {}
    TreeNode(std::string x, std::string n) : GatorID(x), NAME(n), left(nullptr), right(nullptr) {}
};
/*******************************************************************************************/
class AVL
{
public:

    //===================================================//
    /******************** FUNctions **********************/
    //===================================================//
    // Where all the Fun takes place :)

    //========= PRINT FUNctions ===========//
    /* printPostorder, printPreorder, printInorder are mostly the same
    in process, but the cout statement changes place depending
    on the operation

    note the use of a boolean (more like COOLean) to check if it
    printed the first word, if it has we put a comma BEFORE the
    next word printed. that way our last word dosen't have a straggler
    comma after it - kinda neat huh?*/
    void printInorder(TreeNode* root, bool& top)
    {
        if (!root)
            return;
        printInorder(root->left, top);
        if (!top)
            std::cout << ", ";
        top = false;
        std::cout << root->NAME;
        printInorder(root->right, top);
    }
    void printPreorder(TreeNode* root, bool& top)
    {
        if (!root)
            return;
        if (!top)
            std::cout << ", ";
        top = false;
        std::cout << root->NAME;
        printPreorder(root->left, top);
        printPreorder(root->right, top);
    }
    void printPostorder(TreeNode* root, bool& top)
    {
        if (!root)
            return;
        printPostorder(root->left, top);
        printPostorder(root->right, top);
        if (!top)
            std::cout << ", ";
        top = false;
        std::cout << root->NAME;
    }

    /* This fun guy takes in a position key (id) and searches in the tree
    to find the number stored in that inorder position.

    ex:
    if id is 3 the returned node is 25
    if id is 5 the returned node is 77

               70
             /    \
           56      80
          /  \    /  \
         25  60  77  87

     */
    void getNodeInorder(TreeNode* root, int itr, std::string& id)
    {
        static int val = 0;
        if (!root)
            return;
        if (val <= itr)
        {
            getNodeInorder(root->left, itr, id);
            val++;
            if (val == itr)
            {
                id = root->GatorID;
            }
            getNodeInorder(root->right, itr, id);
        }
    }

    /* sums the levels in a tree by using height values in each TreeNode object
    ex:
    level total is 3

               70
             /    \
           56      80
          /  \    /  \
         25  60  77  87

     */
    void printLevelTotal(TreeNode* root)
    {
        if (!root)
        {
            std::cout << 0 << std::endl;
            return;
        }
        else
        {
            std::cout << std::max(height(root->left), height(root->right)) + 1 << std::endl;
            return;
        }
    }

    //============= Search ===============//
    /*
    Search by ID: find a name by using a gatorID and printing the name it associates to.
    no repete ID's so only one person can have an id and that name will print. if the
    ID is not fount, print unsucessful

    Search by NAME: find an ID by using a NAME and printing the number associated to
    that name. There can be multiple people with the same name so when searching by NAME
    every ID held by a person with the matching name will print
    ex:
    search for 44444444 returns "gip"
    search for "geff" returns 22222222

                                     geophff 77777777

                                 /                   \

                 geff 22222222                           jeff 11111111

                  /           \                         /           \

      jieph 88888888       greg 99999999       gip 44444444      jeffery 33333333

     */
    void searchViaID(TreeNode* root, int val)
    {
        if (!root)
        {
            std::cout << "unsuccessful" << std::endl;
            return;
        }

        std::queue<TreeNode*>tree;
        tree.push(root);

        TreeNode* nodeSeeked;

        while (!tree.empty())
        {
            std::string s = tree.front()->GatorID;
            int j = (s.length() / 8);
            for (int i = 0; i < j; i++)
            {
                //cout << "test if name == this id: " << stoi(s.substr(0, 8)) << endl;
                if (stoi(s.substr(0, 8)) == val)
                {
                    nodeSeeked = tree.front();
                    std::cout << nodeSeeked->NAME << std::endl;
                    return;
                }
                s = s.substr(8, s.length());
            }

            if (tree.front()->left)
                tree.push(tree.front()->left);
            if (tree.front()->right)
                tree.push(tree.front()->right);
            tree.pop();
        }

        std::cout << "unsuccessful" << std::endl;
        return;

    }
    void searchViaName(TreeNode* root, std::string val)
    {
        if (!root)
        {
            std::cout << "unsuccessful" << std::endl;
            return;
        }

        std::queue<TreeNode*>tree;
        bool sucess = false;
        tree.push(root);

        TreeNode* nodeSeeked;

        while (!tree.empty())
        {
            if (tree.front()->NAME == val)
            {
                nodeSeeked = tree.front();
                std::cout << nodeSeeked->GatorID << std::endl;
                sucess = true;
            }

            if (tree.front()->left)
                tree.push(tree.front()->left);
            if (tree.front()->right)
                tree.push(tree.front()->right);

            tree.pop();
        }
        if (!sucess)
            std::cout << "unsuccessful" << std::endl;
        return;
    }
    /*used in remove in the case we have to remove a node with 2 children*/
    TreeNode* lowNode(TreeNode* root)
    {
        TreeNode* itr = root;

        /* loop down to find the leftmost leaf */
        while (itr && itr->left)
            itr = itr->left;

        return itr;
    }

    /* Here we take in a Gator ID (id) and search in the tree
    to find the number and delete the corosponding note,returning said
    node

    ViaID will not need to worry about rotations in this situation as per
    the instructions. The rest is mostly reasigning pointers and freeing memory.
    We look for the number of children for the removal node and delete based on
    that information
     */
    TreeNode* removeViaID(TreeNode* root, int id)
    {
        // base case
        if (!root)
            return nullptr;

        else if (id < stoi(root->GatorID.substr(0, 8)))
            root->left = removeViaID(root->left, id);
        else if (id > stoi(root->GatorID.substr(0, 8)))
            root->right = removeViaID(root->right, id);

        //item to delete found
        else if (id == stoi(root->GatorID.substr(0, 8)))
        {
            // Case 1:  No child
            if (!root->left && !root->right) {
                delete root;
                root = nullptr;
            }
            //Case 2: One child 
            else if (!root->left) {
                TreeNode* temp = root;
                root = root->right;
                delete temp;
            }
            else if (root->right == NULL) {
                TreeNode* temp = root;
                root = root->left;
                delete temp;
            }
            // case 3: 2 children
            else {
                TreeNode* temp = lowNode(root->right);
                root->GatorID = temp->GatorID;
                root->NAME = temp->NAME;
                root->right = removeViaID(root->right, stoi(temp->GatorID));
            }

        }
        else
        {
            std::cout << "unsucessful" << std::endl;
            return root;
        }
        return root;
    }

    //========== MANIPULATORS ============//
    /*IsNumber/name both check to see if the input given is valid
    (it checks each char of the string to see if the Gator id has any non
    number characters and ifthe name has any non letter/space characters
    it returns a bool. if they are valid, the new input will be inserted in the tree
    if not, it will print unsucessul in the function calling these two*/
    bool isNumber(std::string s)
    {
        for (int i = 0; i < s.length(); i++)
            if (isdigit(s[i]) == false)
                return false;
        return true;
    }
    bool isName(std::string s)
    {
        for (int i = 0; i < s.length(); i++)
            if (isdigit(s[i]))
                return false;
        return true;
    }

    /*this is used to get the height of a node in a tree, returning
    0 if the root is null*/
    int height(TreeNode* root)
    {
        if (!root)
            return 0;
        return (root->height);
    }

    /* is called in insert/delete where the height of the tree is calculated after changes*/
    TreeNode* newHeight(TreeNode* root)
    {
        root->height = 1 + std::max(height(root->left), height(root->right));
        return  root;
    }
    //===================================================//
    /******************** Rotations **********************/
    //===================================================//

    /* These rotation bois take a note from insert and reasign
    the pointers to balance the tree, returning the new tree re-balanced*/

    TreeNode* rightRotate(TreeNode* root)
    {
        TreeNode* newParent = root->left;
        TreeNode* grandchild = newParent->right;

        newParent->right = root;
        root->left = grandchild;

        // Update heights //
        root = newHeight(root);
        newParent = newHeight(newParent);

        return newParent;
    }

    TreeNode* leftRotate(TreeNode* root)
    {
        TreeNode* grandchild = root->right->left;
        TreeNode* newParent = root->right;

        newParent->left = root;
        root->right = grandchild;

        // Update heights //
        root = newHeight(root);
        newParent = newHeight(newParent);
        return newParent;

    }
    //===================================================//
    /********************* INSERT ************************/
    //===================================================//

    /* This function takes in the input and adds a new TreeNode
    (if the id isn't a copy of an existing one)
     it uses the name and number variables to make a new TreeNode and adds
     it to the tree.

     it cfirst finds the locationof insertion using recursion, then
     inserts and calls the rotate functions to balance the tree */
    TreeNode* insert(std::string name, std::string number, TreeNode* root)
    {
        if (!root)
        {
            std::cout << "successful" << std::endl;  // returns a new node
            return new TreeNode(number, name);
        }

        if (stoi(number) == stoi(root->GatorID))
        {
            std::cout << "unsucessful" << std::endl;
            return root;
        }
        //if # is greater than than the Gator ID, recursion with the right node
        if (stoi(number) > stoi(root->GatorID))
            root->right = insert(name, number, root->right);
        //if # is less than the Gator ID, recursion with the left node
        if (stoi(number) < stoi(root->GatorID))
            root->left = insert(name, number, root->left);

        //Update height of the node
        root = newHeight(root);

        // Get the balance factor and check to see if we gotta rotate:
        int balance = height(root->left) - height(root->right);

        // LR Case
        if (balance > 1 && stoi(number) > (stoi(root->left->GatorID)))
        {
            root->left = leftRotate(root->left);
            return rightRotate(root);
        }
        // RR Case
        if (balance > 1 && stoi(number) < stoi(root->left->GatorID))
            return rightRotate(root);
        // LL Case
        if (balance < -1 && stoi(number) > stoi(root->right->GatorID))
            return leftRotate(root);
        // RL Case
        if (balance < -1 && stoi(number) < stoi(root->right->GatorID))
        {
            root->right = rightRotate(root->right);
            return leftRotate(root);
        }
        return root;
    }
};

//===================================================//
/***************** Call Operations ******************/
//===================================================//
/* to clean up the main function I outsourced calling the the opeartions to here */

/* Uses quoted() to get the name even if it contains a space - very nice
 we call isname/number to check the inserted values are viable and
 call insert if so.

 */
TreeNode* CallInsertOperation(TreeNode* root)
{
    std::stringstream in;
    AVL avl;
    std::string  name, number;
    std::cout << "enter ID Name" << std::endl;
    std::cin >> std::quoted(name);
    std::cout << "enter ID Number" << std::endl;
    std::cin >> number;
    TreeNode* temp = root;
    if (avl.isName(name) && avl.isNumber(number) && number.length() == 8)
        root = avl.insert(name, number, root);
    else
        std::cout << "unsucessful" << std::endl;
    return root;
}
/* search can be a name OR a number, so here we have to find out which it is.
 I call isname to see if it is a name (using substr to remove the "" and
 spaces around the value) and isnumber to check if it is an ID

 we are given the root and use that for our search. */
void callSearchOpperation(TreeNode* root)
{
    AVL avl;
    std::stringstream in;
    std::string search;
    getline(std::cin, search);
    if (avl.isNumber(search.substr(1, (search.length() - 1))))
    {
        if ((search.substr(1, (search.length() - 1))).length() == 8)
            avl.searchViaID(root, stoi(search.substr(1, (search.length() - 1))));
    }
    else if (avl.isName(search.substr(2, (search.length() - 3))))
        avl.searchViaName(root, search.substr(2, (search.length() - 3)));
    else
        std::cout << "unsucessful" << std::endl;
}

/* calls the remove operation where we get the number input and check if it is valid */
TreeNode* callRemoveOperation(TreeNode* root)
{
    AVL avl;
    std::string search;
    std::cin >> search;
    if (avl.isNumber(search))
    {
        root = avl.removeViaID(root, atoi(search.c_str()));
        std::cout << "successful" << std::endl;
    }
    else
        std::cout << "unsucessful" << std::endl;
    return root;

}

/* calls the remove inorder operation where we get the number input, add 1
because of the tree starting at 1, and call getNodeInorder to find the node
from its position. Now that we have the actual node, weuse the returned
node and insert it in removeviaid*/
TreeNode* callRemoveInOrder(TreeNode* root)
{
    AVL avl;
    std::string search;
    std::cin >> search;
    int val = (std::stoi(search) + 1);
    std::string id;
    avl.getNodeInorder(root, val, id);
    root = avl.removeViaID(root, stoi(id));
    std::cout << "successful" << std::endl;
    return root;
}

/* all print operations call their respective
print function with a bool to print the comma correctly

see the print functions for a deeper explanation on this*/
void callPrintInOrderOperation(TreeNode* root)
{
    AVL avl;
    bool top = true;
    avl.printInorder(root, top);
    std::cout << std::endl;
}
void callPrintPreorderOperation(TreeNode* root)
{
    AVL avl;
    bool top = true;
    avl.printPreorder(root, top);
    std::cout << std::endl;
}
void callPrintPostorderOperation(TreeNode* root)
{
    AVL avl;
    bool top = true;
    avl.printPostorder(root, top);
    std::cout << std::endl;
}
//===================================================//
/*********************** MAIN ************************/
//===================================================//

/*The main, the myth, the legend!

we create an AST object and use a TreeNode ptr to call operations.

we take the first inserted number, itr, and go through a while loop
for every operation given*/
int main()
{
    AVL avl;
    std::string operation;
    TreeNode* root = nullptr;
    std::string search;

    //get number of operations:
    std::cout << "input number of opperations" << std::endl;
    int itr;
    std::cin >> itr;

    while (itr--)
    {
        std::cout << "input opperation type: ('search', 'remove', 'printLevelCount', 'printInorder', 'printPreorder', 'printPostorder', 'removeInorder')" << std::endl;
        std::cin >> operation;
        if (operation == "insert")
            root = CallInsertOperation(root);
        else if (operation == "search")
            callSearchOpperation(root);
        else if (operation == "remove")
            root = callRemoveOperation(root);
        else if (operation == "printLevelCount")
            avl.printLevelTotal(root);
        else if (operation == "printInorder")
            callPrintInOrderOperation(root);
        else if (operation == "printPreorder")
            callPrintPreorderOperation(root);
        else if (operation == "printPostorder")
            callPrintPostorderOperation(root);
        else if (operation == "removeInorder")
            root = callRemoveInOrder(root);
    }
}


/*
 ----O(n) complexity----

Insert ~ O(logn):
This operation first calls CallInsertOperation where the inputs 'name' and 'id' are taken
and checked to see if they are valid in isName and isNumber respectively (these functions
use isdigit to test the input, returning a bool). isName's time complexity depends on the
length of the name, but will most likely be negligable with a few checks around O(1) though
it is technically possible to have O(n) with an insanely long name - but highly doubtful to
actually happen. isNumber's complexity will always be O(1) as the input must be 18 digits.
With CallInsertOperation mostly only taking the two inputs and calling isNumber/isName it
checks out at what I believe to be a time complexity of  O(1). We then call insert where
we see the rotation operations (left and right rotate). these operations take constant
time as only a few pointer changes occur: time complexity of O(1). We mainly recursively
iterate through the tree to find where to insert the data; because this tree is balanced
and we are looking for one pot based on ID size, the time complexity is O(logn). As for the
other called functions, height is O(1) as it simply one operation and addHeight is only a
quick O(1) sum of the max height + 1.

With the actual recursion being the largest and most impactful part of this operation
the time complexity is O(logn)
----------------------------------------------------------------------------------
remove:
by ID ~ O(nlogn) -
The time complexity is akin to that of insert as remove follows a simular process.
We must search the tree for the target node for deletion - this is O(logn) in a
balanced tree. we don't need to worry about rotations in this situation as per
the instructions. The rest is mostly reasigning pointers and freeing memory
which is O(1) stuff. But if the child to remove has two nodes, we must
recursively call the function to fix the pointers, which is another O(n)
dependant on the tree's height/ number of nodes.

With the O(logn) for the recursion on nodes with multiple children and the
O(logn) for finding the node to remove, this function is of time complexity
O(nlogn).

by In order ~ O(nlogn) -
exact same as by ID but we firstfind the note at the given position in a
O(logn) time complexity based on the number of nodes. The result is then
run through search byID resulting in a time comlexity of the other - O(nlogn).
------------------------------------------------------------------------------------
search (depends if by ID or name):
search by name ~ O(n)-
While a search operation for finding a value in a AVL is usually O(logn) we have to
consider the fact that this program must support duplicate names with seperate
ID's, as such It must travese the entire tree as there is a posibility for any
number of duplicates, therefor, it is a O(n) operation.

search by ID ~ O(logn)-
There are no duplicate ID's, so the tree undergoes a binary type search in which
we accomplish a O(logn) time complexity.
------------------------------------------------------------------------------------
print:
printPostorder, printPreorder, printInorder ~ O(n) -
the three above operations are the same process with only the location of
the cout changed. They are all traversals and print all the data which is an O(n)
time complexity - based on the number of items to print.

printLevelTotal ~ O(nlogn)
only checks in a binary fashion for a next node and sums the result. this is a
binary type search so it is O(logn)






I love u, have a good day today!!!
*/