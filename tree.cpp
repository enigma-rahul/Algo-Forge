#include "../../include/algorithms/tree.h"
#include "../../include/StepRecorder.h"
#include <sstream>

namespace algo {
namespace tree {

namespace {
struct Node {
    int val;
    Node* left = nullptr;
    Node* right = nullptr;
    explicit Node(int v) : val(v) {}
};

void freeTree(Node* root) {
    if (!root) return;
    freeTree(root->left);
    freeTree(root->right);
    delete root;
}
} // anonymous namespace

AlgorithmResult bstInsertAndSearch(const std::vector<int>& keys, int searchTarget) {
    StepRecorder recorder;
    AlgorithmResult res;
    res.algorithm = "bst";
    res.input = {{"keys", keys}, {"search_target", searchTarget}};

    if (keys.empty()) {
        res.success = false;
        res.error_message = "BST requires at least one key to insert.";
        return res;
    }

    Node* root = nullptr;

    // Step 1: Insert keys into BST
    for (int key : keys) {
        if (!root) {
            root = new Node(key);
            std::ostringstream msg;
            msg << "Created BST root node with key " << key;
            recorder.recordTreeInsert(key, msg.str());
        } else {
            Node* curr = root;
            while (true) {
                if (key < curr->val) {
                    if (!curr->left) {
                        curr->left = new Node(key);
                        std::ostringstream msg;
                        msg << "Inserted key " << key << " as left child of " << curr->val;
                        recorder.recordTreeInsert(key, msg.str());
                        break;
                    }
                    curr = curr->left;
                } else if (key > curr->val) {
                    if (!curr->right) {
                        curr->right = new Node(key);
                        std::ostringstream msg;
                        msg << "Inserted key " << key << " as right child of " << curr->val;
                        recorder.recordTreeInsert(key, msg.str());
                        break;
                    }
                    curr = curr->right;
                } else {
                    // Duplicate key ignored
                    break;
                }
            }
        }
    }

    // Step 2: Search for target in BST
    Node* curr = root;
    bool found = false;
    std::vector<int> searchPath;

    while (curr) {
        searchPath.push_back(curr->val);
        if (curr->val == searchTarget) {
            std::ostringstream msg;
            msg << "Found target key " << searchTarget << " in BST!";
            recorder.recordFound(0, searchTarget, msg.str());
            found = true;
            break;
        } else if (searchTarget < curr->val) {
            std::ostringstream msg;
            msg << "Key " << searchTarget << " < node " << curr->val << ": branching left";
            recorder.recordTreeSearchStep(curr->val, "left", msg.str());
            curr = curr->left;
        } else {
            std::ostringstream msg;
            msg << "Key " << searchTarget << " > node " << curr->val << ": branching right";
            recorder.recordTreeSearchStep(curr->val, "right", msg.str());
            curr = curr->right;
        }
    }

    if (!found) {
        std::ostringstream msg;
        msg << "Target key " << searchTarget << " was not found in the BST.";
        recorder.recordNotFound(searchTarget, msg.str());
    }

    freeTree(root);

    recorder.recordDone(searchPath, found ? "BST search succeeded!" : "BST search completed (key absent)");
    res.result = {{"found", found}, {"search_path", searchPath}};
    res.steps = recorder.getSteps();
    return res;
}

} // namespace tree
} // namespace algo
