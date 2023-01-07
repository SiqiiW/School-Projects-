package project5;

/**
 * This class represents a hiker traveling down the mountain.
 * An object of this class is capable of keeping track of all the supplies that the hiker has in their possession.
 * This information should be updated as the hiker travels along a trail and consumes supplies
 * (by either eating along the way, or using the tools to clear the path, or cross the river).
 * 
 * @author Siqi Wang
 *
 */

public class Hiker {
	
	//Define three variables keeping track of the amount of each kind of supplies
	private int AmountOfFood = 0;
	private int AmountOfRaft = 0;
	private int AmountOfAxe = 0;
	
	//create a counter that keeps track of number of times supplies have been updated
	private int counter = 0;


	//constructors
	public Hiker() {
		
	}
	
	public Hiker(int amountOfFood, int amountOfRaft, int amountOfAxe, int Counter) {
		super();
		AmountOfFood = amountOfFood;
		AmountOfRaft = amountOfRaft;
		AmountOfAxe = amountOfAxe;
		counter = Counter;
	}





	/**
	 * This method updates the amount of supplies every time we land on a new rest stop.
	 * Hiker can use the supplies in one rest stop to overcome obstacles at the same rest stop.
	 * @param newRestStop the new rest stop we are going to land on
	 * @return true if the rest stop could be passed
	 *         false if hiker doesn't have enough supply to pass it
	 */
	public boolean updateSupplies(RestStop newRestStop) {
		//if the rest stop doesn't exist
		if (newRestStop == null) 
			return false; //straightly terminate this update method
		
		//store the original value of food, axe and raft
		int originalFood = AmountOfFood;
		int originalAxe = AmountOfAxe;
		int originalRaft = AmountOfRaft;
		
		
		//to get to this rest stop, hiker consumes one food
		//if this rest stop is the root node of the tree, then hiker doesn't need to consume food
		if (counter > 0)
			AmountOfFood --;
		
		counter++;	
		
		//update the supplies according to the supplies and obstacles at this rest stop 
		for (String str : newRestStop.getSupplies()) {
			if (str.equalsIgnoreCase("food"))
				AmountOfFood ++;
			else if (str.equalsIgnoreCase("raft"))
				AmountOfRaft ++;
			else if (str.equalsIgnoreCase("axe"))
				AmountOfAxe ++;
		}
		for (String str : newRestStop.getObstacles()) {
			if (str.equalsIgnoreCase("river"))
				AmountOfRaft --;
			else if (str.equalsIgnoreCase("fallen tree"))
				AmountOfAxe --;
		}
			

		//check if the hiker has enough supplies to pass this rest stop
		if (AmountOfFood < 0 || AmountOfRaft < 0 || AmountOfAxe < 0) {
			//if the hiker couldn't reach the current rest stop, don't update the supplies
			AmountOfFood = originalFood;		
			AmountOfAxe = originalAxe;		
			AmountOfRaft = originalRaft;				
			return false;
		}
		else {
			return true;		

		}
	}
	
	
	//the accessors and mutators for variables
	public int getAmountOfFood() {
		return AmountOfFood;
	}

	public void setAmountOfFood(int amountOfFood) {
		AmountOfFood = amountOfFood;
	}

	public int getAmountOfRaft() {
		return AmountOfRaft;
	}

	public void setAmountOfRaft(int amountOfRaft) {
		AmountOfRaft = amountOfRaft;
	}

	public int getAmountOfAxe() {
		return AmountOfAxe;
	}

	public void setAmountOfAxe(int amountOfAxe) {
		AmountOfAxe = amountOfAxe;
	}
	
	public int getCounter() {
		return counter;
	}

	
	

}