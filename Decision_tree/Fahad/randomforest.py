# %%
import numpy as np 
import pandas as pd 
from sklearn.preprocessing import LabelEncoder


data = pd.read_csv('Breast_cancer_data.csv')

## converting categorical value to numerical 

categories = []
# categories = ['Outlook','Temp','Humidity','Wind','Play'] # weather
# categories = ['Color','Size','Act','Age','Inflated'] # ballons
# categories = ['top-left-square','top-middle-square','top-right-square','middle-left-square','middle-middle-square','middle-right-square','bottom-left-square','bottom-middle-square','bottom-right-square','Class']
target = ['Yes', 'No']
 
# Creating a instance of label Encoder.
le = LabelEncoder()
 
for i in range(len(categories)):
    # Using .fit_transform function to fit label
    # encoder and return encoded label
    label = le.fit_transform(data[categories[i]])

    # removing the column 'Purchased' from df
    # as it is of no use now.
    data.drop(categories[i], axis=1, inplace=True)
    
    # Appending the array to our dataFrame
    # with column name 'Purchased'
    data[categories[i]] = label 

# %%
import numpy as np
from collections import Counter 

class Node:
    def __init__(self, feature=None, threshold=None, left=None, right=None,*, value=None) -> None:
        self.feature = feature
        self.threshold = threshold
        self.left = left
        self.right = right
        self.value = value # leaf node 
    
    def if_leaf_node(self):
        # when the node has any value, it will return True
        return self.value is not None

# %%
class MyDecisionTree:
    
    def __init__(self, min_sample_split=2, max_depth=10, n_features=None) -> None:
        
        # minimum number of samples required to split an internal node
        self.min_sample_split = min_sample_split 
        
        # maximum depth (levels) of the tree
        self.max_depth = max_depth 
        
        # number of features to consider when looking for the best split 
        self.n_features = n_features
        
        # tree root 
        self.root = None
        
    def fit(self, X, y):
                
        # check if the number of features doesn't exceed the number of features in the dataset
        self.n_features = X.shape[1] if not self.n_features else min(self.n_features, X.shape[1])
        
        # we'll build the tree recursively starting from the root node
        self.root = self._build_tree(X, y)
                
    def _build_tree(self, X, y, depth=0):
        
        n_samples, n_features = X.shape     # [rows, columns]
        n_labels = len(np.unique(y))        # class variables
        
        # check the stopping criteria 
        if (depth >= self.max_depth or n_labels == 1 or n_samples < self.min_sample_split):
            
            if len(y) > 0:
                leaf_node_value = self.calculate_leaf_value(y) 
                return Node(value=leaf_node_value)
            else: return Node(value=0)
        
        # we set the replace to False to get the unique features
        feature_indices = np.random.choice(n_features, self.n_features, replace=False)
                
        # find the best split 
        best_feature, best_threshold = self._find_best_split(X, y, feature_indices)
        
        # create child nodes
        left_indices, right_indices = self._split_node(X[:, best_feature], best_threshold)
        left = self._build_tree(X[left_indices, :], y[left_indices], depth+1)
        right = self._build_tree(X[right_indices, :], y[right_indices], depth+1)
        return Node(best_feature, best_threshold, left, right)
        
    def calculate_leaf_value(self, y):
          
        counter = Counter(y)
        value = counter.most_common(1)[0][0]
        
        return value 
    
    def _find_best_split(self, X, y, feature_indices):
        
        best_gain = -1 
        split_index, split_threshold = None, None
        
        for feature_index in feature_indices:
            
            # get the feature_index column value for all rows, [row, column]
            X_column = X[:, feature_index]  
            thresholds = np.unique(X_column)
            
            # find information gain for each of the threshold values
            for threshold in thresholds:
                gain = self._information_gain(y, X_column, threshold)
                
                if gain > best_gain:
                    best_gain = gain 
                    split_index = feature_index
                    split_threshold = threshold
        
        return split_index, split_threshold     
        
    def _information_gain(self, y, X_column, threshold):
        
        # parent entropy
        parent_entropy = self._entropy(y)
        
        # create children
        left_indices, right_indices = self._split_node(X_column, threshold)
        
        if len(left_indices) == 0 or len(right_indices) == 0:
            return 0
        
        # calculate weighted entropy of children
        total_samples = len(y)
        left_subtree_samples, right_subtree_samples = len(left_indices), len(right_indices)
        entropy_left_subtree, entropy_right_subtree = self._entropy(y[left_indices]), self._entropy(y[right_indices])
        child_entropy = (left_subtree_samples/total_samples) * entropy_left_subtree + (right_subtree_samples/total_samples) * entropy_right_subtree
        
        # calculate information gain
        information_gain = parent_entropy - child_entropy
        return information_gain
        
    def _entropy(self, y):
        
        hist = np.bincount(y) # count the number of each class variable in the array
        ps = hist/len(y)      # probability of each class variable (#p/n)
        
        return -np.sum([p * np.log(p) for p in ps if p>0])
        
    def _split_node(self, X_column, threshold):
        
        # Use argwhere to find the indices of the elements in X_column that are less than or equal to the threshold
        # Flatten the resulting 2D array of indices into a 1D array
        
        left_indices = np.argwhere(X_column <= threshold).flatten()
        right_indices = np.argwhere(X_column > threshold).flatten()
        return left_indices, right_indices
            
    def predict(self, X):
        return np.array([self._traverse_tree(x, self.root) for x in X])
            
    def _traverse_tree(self, x, node):
        
        if node.if_leaf_node():
            return node.value
        
        if x[node.feature] <= node.threshold:  
            return self._traverse_tree(x, node.left)
        
        else:
            return self._traverse_tree(x, node.right)
        
    def print_tree(self, root):
        self._print_node(root)
        
    def _print_node(self, node, indent=""):
        if node is None:
            return
        if node.if_leaf_node():
            print(indent + "Leaf Node: " + str(node.value))
        else:
            print(indent + "Feature [LEFT] " + categories[(node.feature)] + " <= " + target[node.threshold] + "?")
            self._print_node(node.left, indent + "|__")
            print(indent + "Feature [RIGHT] " + categories[(node.feature)] )
            self._print_node(node.right, indent + "|__")
               

# %%
# Random Tree from Scratch
from sklearn.model_selection import train_test_split
from sklearn.metrics import confusion_matrix , accuracy_score


class RandomForest:
    
    def __init__(self, n_estimators=11, max_depth=10, n_features=3) -> None:
        
        # number of decision trees
        self.n_estimators = n_estimators
        
        self.max_depth = max_depth
        self.n_features = n_features
        self.tree = []
        
       
    def fit(self, X, y):
        
        for _ in range(self.n_estimators):
            
            decision_tree = MyDecisionTree(n_features=self.n_features)
            
            # Randomly select a subset of the data with replacement
            sample_indices = np.random.choice(len(X), len(X), replace=True)
            X_sample = X[sample_indices]
            y_sample = y[sample_indices]
            
            decision_tree.fit(X_sample, y_sample)
            self.tree.append(decision_tree)
    
    def predict(self, X):
        
        # Stack the lists vertically into a numpy array
        arr = np.vstack([tree.predict(X) for tree in self.tree])

        # Define a function to find the most frequent value in a numpy array
        def most_frequent_value(arr):
            return np.argmax(np.bincount(arr))

        # Apply the function along the columns of the array
        result = np.apply_along_axis(most_frequent_value, axis=0, arr=arr)
        return result
                

# %%
# predict using random forest     
train, test = train_test_split(data, test_size=.4)

X_test, Y_test = test.iloc[:,:-1].values, test.iloc[:,-1].values
X_train, Y_train = train.iloc[:,:-1].values, train.iloc[:,-1].values

random_forest = RandomForest()
random_forest.fit(X_train, Y_train)
Y_pred = random_forest.predict(X_test)
print(Y_pred)

# print the confusion matrix and accuracy
print("\nConfusion Matrix: \n", confusion_matrix(Y_test, Y_pred))
print("Accuracy: ",accuracy_score(Y_test, Y_pred)*100, "%")  

# %%
# Decision Tree from sklearn
from sklearn.ensemble import RandomForestClassifier
clf = RandomForestClassifier(random_state=42, n_jobs=-1, max_depth=5, n_estimators=100, oob_score=True)

clf.fit(X_train, Y_train)
Y_pred = clf.predict(X_test)
print(Y_pred)

print("Confusion Matrix: \n", confusion_matrix(Y_test, Y_pred))
print("Accuracy: ",accuracy_score(Y_test, Y_pred)*100, "%")


