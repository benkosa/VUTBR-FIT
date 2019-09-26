package ija.ija2018.homework2.common;

public class Figure{
	public String type;
	public String colour;
	public int x;
	public int y;

	public Figure(int x, int y, String type, String colour) {
		this.x = x;
		this.y = y;
		this.type = type;
		this.colour = colour;
	}

	public String getState() {
		return this.type + "[" + this.colour + "]" + this.x + ":" + this.y;
	}	

}
