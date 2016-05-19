/*
1. Given a binary tree and a sum, find all root-to-leaf paths where each path’s sum equals the given sum
Given the below binary tree and sum = 22,
              5
             /  \
           4    8
          /     / \
         11   13   4
        / \         / \
       7   2        5   1
return
[
   [5,4,11,2],
   [5,8,4,5]
]

*/

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
};

vector<vector<int> > pathToSum(TreeNode *root, int sum) {
    vector<vector<int> > retval;
    vector<int> path;
    pathToSum(root, sum, path, retval);
    return retval;
}

void pathToSum(TreeNode* root, int sum, vector<int> &path, vector<vector<int> > &retval) {
    if (root == NULL) {
        return;
    }
    
    if (root->val <= sum) {
        
        path.push_back(root->val);
        
        sum -= root->val;
        if (sum == 0) {
            if (root->left == NULL && root->right == NULL) {
                retval.push_back(path);
	    }

            path.pop();
            return;
        }
        
        pathToSum(root->left, sum, path, retval);
        pathToSum(root->right, sum, path, retval);
        path.pop();
    }
}

 
