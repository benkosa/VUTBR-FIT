package ija.ija2018.homework1.board;

public interface Field {
	
	public static enum Direction{
		D,
		L,
		LD,
		LU,
		R,
		RD,
		RU,
		U
	}
	
	
	void addNextField(Field.Direction dirs, Field field);
	
	Disk get();
	
	Field nextField(Field.Direction dirs);
	
	boolean put(Disk disk);

	boolean remove(Disk disk);

	boolean isEmpty();


}

