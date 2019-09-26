package ija.ija2018.homework2.game;

import java.util.ArrayList;

import ija.ija2018.homework2.common.Field;
import ija.ija2018.homework2.common.Figure;
import ija.ija2018.homework2.common.Game;
import ija.ija2018.homework2.common.Undo;



public class Board extends java.lang.Object implements Game{
	private int size = 0;
	private Field[][] array = new Field[10][10];
	private ArrayList<Undo> history = new ArrayList<Undo>();
	public String game_type = null;
	
	public Board(int size) {
		this.size = size+1;
		
		for(int i = 1; i < this.size; i++) {
			for(int j = 1; j < this.size; j++) {
				this.array[i][j] = new Field();
				this.array[i][j].x = i;
				this.array[i][j].y = j;
			}
		}
	}

	public Field getField(int i, int j) {
		return this.array[i][j];
	}
	
	public void addFigure(int i, int j, String type, String colour) {
		this.array[i][j].figureType = new Figure(i, j, type, colour);
		this.array[i][j].isEmpty = false;
	}

	public boolean move(Figure figure, Field field) {
		if(this.game_type == "D") {
			String prew_colour = null;
			String prew_type = null;	
			if(field.figureType!= null) {
				prew_colour = field.figureType.colour;
				prew_type = field.figureType.type;
			}
			Undo undo = new Undo(figure, field, prew_colour, prew_type);
			boolean ret = this.moveD(figure, field);
			if(ret == true)this.history.add(undo);
			if(ret == true)figure.x = field.x;
			if(ret == true)figure.y = field.y;
			return ret;
		}
		
		if(figure.type == "P") {
			String prew_colour = null;
			String prew_type = null;
			if(field.figureType!= null) {
				prew_colour = field.figureType.colour;
				prew_type = field.figureType.type;
			}
			Undo undo = new Undo(figure, field, prew_colour, prew_type);
			boolean ret = this.moveP(figure, field);
			if(ret == true)this.history.add(undo);
			if(ret == true)figure.x = field.x;
			if(ret == true)figure.y = field.y;
			return ret;
		}
		if(figure.type == "V") {
			String prew_colour = null;
			String prew_type = null;
			if(field.figureType!= null) {
				prew_colour = field.figureType.colour;
				prew_type = field.figureType.type;
			}
			Undo undo = new Undo(figure, field, prew_colour, prew_type);
			boolean ret = this.moveV(figure, field);
			if(ret == true)this.history.add(undo);
			if(ret == true)figure.x = field.x;
			if(ret == true)figure.y = field.y;
			return ret;
		}
		return false;
	}
	
	private boolean moveD(Figure figure, Field field) {
		int moveType;
		if (figure.colour == "W") moveType = 1; else moveType = -1;
		
		if(figure.y + moveType == field.y && figure.x + 1 == field.x) {//posun do prava
			if(field.isEmpty == false) return false;
			
			this.addFigure(field.x, field.y, figure.type, figure.colour);
			this.array[figure.x][figure.y].isEmpty = true;
			return true;
		}
		if(figure.y + moveType == field.y && figure.x - 1 == field.x) {//posun do lava
			if(field.isEmpty == false) return false;			
			
			this.addFigure(field.x, field.y, figure.type, figure.colour);
			this.array[figure.x][figure.y].isEmpty = true;
			return true;
		}
		return false;
	}

	public boolean moveP(Figure figure, Field field) {
		int moveType;
		if (figure.colour == "W") moveType = 1; else moveType = -1;
		
		if(figure.y + moveType == field.y && figure.x == field.x) {//posun hore
			if(field.isEmpty == false) return false;
			
			this.addFigure(field.x, field.y, figure.type, figure.colour);
			this.array[figure.x][figure.y].isEmpty = true;
			return true;
		}
		if(figure.y + moveType == field.y && figure.x + 1 == field.x) {//vyhodenie do prava
			if(field.isEmpty == true) return false;
			else 
				if(field.figureType.colour == figure.colour) return false;
			
			this.addFigure(field.x, field.y, figure.type, figure.colour);
			this.array[figure.x][figure.y].isEmpty = true;
			return true;
		}
		if(figure.y + moveType == field.y && figure.x - 1 == field.x) {//vyhodenie do lava
			if(field.isEmpty == true) return false;
			else 
				if(field.figureType.colour == figure.colour) return false;			
			
			this.addFigure(field.x, field.y, figure.type, figure.colour);
			this.array[figure.x][figure.y].isEmpty = true;
			return true;
		}
		return false;
	}
	
	public boolean moveV(Figure figure, Field field) {
		if(figure.x == field.x) {
			int max = figure.y;
			int min = field.y;
			if(max < min) {
				max = field.y;
				min = figure.y;
			}
			min++;
			for(int i = min; i < max; ++i) {
				if(this.array[field.x][i].isEmpty == false)
					return false;
			}
			if(field.isEmpty == false)
				if(field.figureType.colour == figure.colour) return false;			
			
			this.addFigure(field.x, field.y, figure.type, figure.colour);
			this.array[figure.x][figure.y].isEmpty = true;
			return true;
		}		
		if(figure.y == field.y) {
			int max = figure.x;
			int min = field.x;
			if(max < min) {
				max = field.x;
				min = figure.x;
			}
			min++;
			for(int i = min; i < max; ++i) {
				if(this.array[i][field.y].isEmpty == false)
					return false;
			}
			
			if(field.isEmpty == false) 
				if(field.figureType.colour == figure.colour) return false;			
			
			this.addFigure(field.x, field.y, figure.type, figure.colour);
			this.array[figure.x][figure.y].isEmpty = true;
			return true;
		}
		return false;
	}

	public void undo() {
		Undo undo = this.history.get(this.history.size()-1);
		Field to_field = this.array[undo.x_field][undo.y_field];
		
		this.array[undo.x_figure][undo.y_figure].figureType = null;
		this.array[undo.x_figure][undo.y_figure].figureType = new Figure(undo.x_figure, undo.y_figure, undo.prew_type, undo.prew_colour);
		if(undo.prew_colour != null)
			this.array[undo.x_figure][undo.y_figure].isEmpty = false;
		
		to_field.figureType.x = to_field.x;
		to_field.figureType.y = to_field.y;

		this.history.remove(this.history.size()-1);
		
	}

}
