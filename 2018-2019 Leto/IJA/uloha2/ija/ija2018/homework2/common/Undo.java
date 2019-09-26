package ija.ija2018.homework2.common;

public class Undo {
	public int x_figure = 0;
	public int y_figure = 0;
	public int x_field = 0;
	public int y_field = 0;
	public String prew_colour;
	public String prew_type;
	
	public Undo(Figure figure, Field field, String prew_colour, String prew_type) {
		this.x_figure = field.x;
		this.y_figure = field.y;
		this.x_field = figure.x;
		this.y_field = figure.y;
		this.prew_colour = prew_colour;
		this.prew_type = prew_type;
	}
	
}
