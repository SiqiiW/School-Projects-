package project5;

import java.util.ArrayList;

/**
 * This class represents a single rest stop.
 * It is capable of storing the label of the rest stop;
 *                          a list of supplies that a hiker can collect at this rest-stop;
 *                          a list of obstacles that a hiker may encounter at this rest-stop.
 * 
 * @author Siqi Wang
 *
 */

public class RestStop{
	
	private String label;
	private ArrayList<String> supplies = new ArrayList<String> ();
	private ArrayList<String> obstacles = new ArrayList<String> ();	

	
	//constructor
	public RestStop(String label, ArrayList<String> SuppliesAndObstacles) {
		for (int i = 0; i < SuppliesAndObstacles.size(); i++) {
			String str = SuppliesAndObstacles.get(i);
			//add the valid supplies and obstacles at this rest stop to the arraylist
			if (str.equalsIgnoreCase("food") ||
					str.equalsIgnoreCase("raft")||
					str.equalsIgnoreCase("axe")) {
				supplies.add(SuppliesAndObstacles.get(i));
			}
			else if (str.equalsIgnoreCase("river")){
				obstacles.add(str);
			}
			else if (str.equalsIgnoreCase("fallen") &&
							SuppliesAndObstacles.get(i + 1).equalsIgnoreCase("tree")){
				obstacles.add(str+ " "+ SuppliesAndObstacles.get(i + 1));				
			}
		}
		this.label = label;
	}


	//accessors and mutators
	public String getLabel() {
		return label;
	}


	public void setLabel(String label) {
		this.label = label;
	}


	public ArrayList<String> getSupplies() {
		return supplies;
	}


	public void setSupplies(ArrayList<String> supplies) {
		this.supplies = supplies;
	}


	public ArrayList<String> getObstacles() {
		return obstacles;
	}


	public void setObstacles(ArrayList<String> obstacles) {
		this.obstacles = obstacles;
	}


}
