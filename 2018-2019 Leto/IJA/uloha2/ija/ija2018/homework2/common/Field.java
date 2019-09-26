package ija.ija2018.homework2.common;

public class Field extends java.lang.Object{
	public Figure figureType = null;
	public int x;
	public int y;
	public boolean isEmpty = true;

	public Figure get() {
		return this.figureType;
	}

	public boolean isEmpty() {			
		return this.isEmpty;
	}
}
