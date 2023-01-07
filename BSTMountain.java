package project5;

import java.util.Comparator;

/**
 * This class reuses the BST<E> class created by professor Joanna Klukowska.
 * The class represents the mountain itself, and its nodes store data items of type RestStop.
 * It implements an actual binary search tree.
 * The methods implemented are the constructor, add, size and maxDepth methods.
 * @author Siqi Wang
 *
 */

public class BSTMountain {
	
    private BSTNode root;   //reference to the root node of the tree 
    private Comparator comparator;   //comparator object to overwrite the 
                                //natural ordering of the elements 
    	
    private boolean added ; //helper variable used by the add method 
    
    private int size; //variable that keeps track of the size


    //accessors and mutators
    public BSTNode getRoot() {
		return root;
	}
	
	

	/**
	 * Constructs a new, empty tree, sorted according to the natural ordering of its elements.
	 */
    public BSTMountain () {
        root = null; 
        comparator = null; 
    }

	/**
	 * Adds the specified element to this tree if it is not already present. 
	 * If this tree already contains the element, the call leaves the 
     * tree unchanged and returns false.
	 * @param data element to be added to this tree 
     * @return true if this tree did not already contain the specified element 
     * @throws NullPointerException if the specified element is null  
	 */
    public boolean add ( RestStop data ) { 
         added = false; 
         if (data == null) return added; 
         //replace root with the reference to the tree after the new 
         //value is added
         root = add (data, root);
         return added; 
    }
    /*
	 * Actual recursive implementation of add. 
     *
     * This function returns a reference to the subtree in which 
     * the new value was added. 
	 *
     * @param data element to be added to this tree 
     * @param node node at which the recursive call is made 
	 */
    private BSTNode add (RestStop data, BSTNode node ) {
        if (node == null) {
            added = true; 
            size++;
            return new BSTNode(data); 
        }
        //decide how comparisons should be done 
        int comp = 0 ;
        if (comparator == null ) //use natural ordering of the elements 
            comp = node.data.getLabel().compareTo(data.getLabel()); 
        else                     //use the comparator 
            comp = comparator.compare(node.data.getLabel(), data.getLabel() ) ;
        
        //find the location to add the new value 
        if (comp > 0 ) { //add to the left subtree 
            node.left = add(data, node.left); 
        }
        else if (comp < 0 ) { //add to the right subtree
            node.right = add(data, node.right); 
        }
        else { //duplicate found, do not add 
            added = false; 
            return node; 
        }
        return node; 
    }

	/**
	 * Returns the number of elements in this tree.
	 * @return the number of elements in this tree
	 */
	public int size() {
		return size;
	}

    /* 
     * Node class for this BST 
     */ 
    public class BSTNode {

        RestStop data;
        BSTNode  left;
        BSTNode  right;
        int depth;

        //constructor
        public BSTNode ( RestStop data ) {
            this.data = data;
        }

        public BSTNode (RestStop data, BSTNode left, BSTNode right ) {
            this.data = data;
            this.left = left;
            this.right = right;
        }
        

        //accessors and mutators
		public RestStop getData() {
			return data;
		}
		
		public int getDepth() {
			return depth;
		}
    }
    
    
    //creates a variable that keeps track of the max depth
    private int maxDepth = 0;
    
    //accessor for this maximum depth
    public int getMaxDepth(){
    	return maxDepth;
    }
    
    /*
     * This method assigns depth to each node in the BST mountain.
     * It also generates the max depth.
     */   
    public void updateDepth(BSTNode node, int Depth){
    	//if we haven't reach beyond the leaf nodes
        if (node != null){
        	//assign depth to this node
            node.depth = Depth;
            //update the max depth
            if (Depth > maxDepth)
            	maxDepth = Depth;
            // update the depth for nodes in left subtree and the right subtree 
            updateDepth(node.left, Depth + 1); 
            updateDepth(node.right, Depth + 1); 
        }
    }
}

