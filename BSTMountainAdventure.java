package project5;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.NoSuchElementException;
import java.util.Scanner;

import project5.BSTMountain.BSTNode;

/**
 * This is the class that is the program. 
 * This means it has the main method. 
 * This class is responsible for parsing and validating the command line arguments, 
 * reading and parsing the input file, producing any error messages, handling any 
 * exceptions thrown by other classes, and producing output.
 * 
 * @author Siqi Wang
 *
 */

public class BSTMountainAdventure {
	
	//create an array list to store all the possible paths
	static ArrayList<String> storePath = new ArrayList<String> ();
	//create a string to store the current path
	static String tmp = "";
	
	/**
	 * A wrapper function that checks error.
	 * @param intArray the puzzle given to solve
	 * @return false if it finds error,
	 *         or the recursive function otherwise.
	 */
	public static boolean FindWay(BSTMountain Mountain) {
		
		BSTNode root = Mountain.getRoot();
		//assign depth to each node in the mountain
		Mountain.updateDepth(root, 1);
		//get the max depth of this BST mountain
		int maxDepth = Mountain.getMaxDepth();
		//create a new hiker to arrive at the root node
		Hiker hiker = new Hiker();
		//checks for empty mountain
		if (root != null && Mountain.size() > 0) {
			//the actual recursive function
			return WayFinderRec(Mountain, root, tmp, maxDepth, hiker);
		}
		else {
			return false;
		}
	}
	
	
	/**
	 * The recursive function that generates paths.
	 * @param Mountain the BST mountain the hiker is climbing
	 * @param node the node with the current rest stop the hiker is stepping on
	 * @param tmp a string that keeps track of the current path
	 * @param maxDepth the maximum depth of the BST mountain
	 * @param hiker the current hiker that is on the road
	 * 
	 * @return true if there is any possible path,
	 *         or false if there isn't any.
	 */
	public static boolean WayFinderRec(BSTMountain Mountain, BSTNode node, String tmp, int maxDepth, Hiker hiker) {

		/**
		 * Update the hiker's supplies.
		 * If the hiker doesn't have enough supplies to land on the current rest stop,
		 * the supplies won't be updated and this path is abandoned.
		 */
		if (!hiker.updateSupplies(node.getData())) {
			return false;
		}
		

		//add the possible new moves to the path string
		String proceed;
		if (tmp.length() != 0)
			proceed =  tmp + " " + node.getData().getLabel();
		else
			proceed = node.getData().getLabel();
		
		//base case
		//checks if we reached the final rest stop of a path
		if (node.left == null && node.right == null) {
			//if this final stop is a cliff, abandon this path
			if (node.getDepth() != maxDepth ) {
				return false;
				
			}
			else{
				if (! proceed.equals(""))
					storePath.add(proceed);
				
			}
			return true;	
		}
		
		
		
		
		//recursive calls			
		
		
		//assign two hikers with same supplies to go different direction 
		Hiker leftH = new Hiker (hiker.getAmountOfFood(), hiker.getAmountOfRaft(), hiker.getAmountOfAxe(), hiker.getCounter());
		Hiker rightH = new Hiker (hiker.getAmountOfFood(), hiker.getAmountOfRaft(), hiker.getAmountOfAxe(), hiker.getCounter());
		
		boolean LeftPossible = false;
		boolean RightPossible = false;		
		
		//if the left hiker can go left, then go left
		if (node.left != null) {
			LeftPossible = WayFinderRec( Mountain, node.left, proceed, maxDepth, leftH ) || false;

		}
		//if the right hiker can go right, then go right
		if (node.right != null) {
			RightPossible = WayFinderRec( Mountain, node.right, proceed, maxDepth, rightH) || false;

		}
		boolean possible = LeftPossible||RightPossible;

		//return true if there is any possible path
		return possible;
	}
	
	
	
	/**
	 * The main() method of this program. 
	 * @param args array of Strings provided on the command line when the program is started; 
	 * the first string should be the name of the input file containing the list of rest stops. 
	 */
	public static void main(String[] args) {
		
		//verify that the command line argument exists 
		if (args.length == 0 ) {
			System.err.println("Usage Error: the program expects file name as an argument.\n");
			System.exit(1);
		}
		
		//verify that command line argument contains a name of an existing file 
		File hikingFile = new File(args[0]); 
		if (!hikingFile.exists()){
			System.err.println("Error: the file "+hikingFile.getAbsolutePath()+" does not exist.\n");
			System.exit(1);
		}
		if (!hikingFile.canRead()){
			System.err.println("Error: the file "+hikingFile.getAbsolutePath()+
											" cannot be opened for reading.\n");
			System.exit(1);
		}
		
		//open the file for reading 
		Scanner inStops = null; 
		
		//error checking
		try {
			inStops = new Scanner ( hikingFile ) ;
		} catch (FileNotFoundException e) {
			System.err.println("Error: the file "+hikingFile.getAbsolutePath()+
											" cannot be opened for reading.\n");
			System.exit(1);
		}
		
		//read the content of the file and save the data in a list of named colors
		BSTMountain Tree = new BSTMountain(); 
		String line = null; 
		Scanner parseLine = null; 
		String Label = null;
		ArrayList<String> SuppliesAndObstacles = new ArrayList<String> ();
		RestStop current = null;
		while (inStops.hasNextLine()) {
			//Empty the list of supplies and obstacles from the last rest stop
			SuppliesAndObstacles.clear();
			try { 
				int counter = 0;
				line = inStops.nextLine(); 
				parseLine = new Scanner(line);
				Label = parseLine.next();
				while (parseLine.hasNext()) {
					SuppliesAndObstacles.add(counter, parseLine.next());
					counter++;
				}
			}
			catch (NoSuchElementException ex ) {
				//caused by an incomplete or miss-formatted line in the input file
				System.err.println(line);
				continue; 	
			}
			try {
				//add the rest stop to the tree
				//invalid supplies and obstacles are handled in RestStop class, inside the constructor
				current = new RestStop (Label, SuppliesAndObstacles); 
				Tree.add(  current  ); 
			}
			catch (IllegalArgumentException ex ) {
				//ignore this exception and skip to the next line 
			}
		}
		
		//call the recursive function
		FindWay(Tree);
		
		//see if the recursive function generates any path
		if (storePath.size() > 0) {		
			//print out the found paths
			for (String s : storePath) {
				System.out.println(s);
			}
		}
	}
}
