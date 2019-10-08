package ija.ija2018.homework1.board;

public class Disk extends java.lang.Object {

	private boolean disk;
	public Disk(boolean isWhite) 
	{
		disk = isWhite;
	}
	
	public void turn()
	{
		if(disk) disk = false;
		else disk = true;
	}
	
	public boolean isWhite()
	{
		if(disk) return true;
		else return false;
	}
	
	private boolean findDisk(Field.Direction dirs, Field moveTo) 
	{
		Field[] neighbor = new Field[8];
		neighbor[0] = moveTo.nextField(dirs);
		if(neighbor[0] == null) return false;
		if(neighbor[0].get() != null)
			if(neighbor[0].get().equals(this) == true) {
				neighbor[0].remove(neighbor[0].get());
				return true;
			}else return false;
		
		for(int i = 0; i < 7; i++) {
			neighbor[i+1] = neighbor[i].nextField(dirs);
			if(neighbor[i+1] == null) return false;
			if(neighbor[i+1].get() != null)
				if(neighbor[i+1].get().equals(this) == true) {
					neighbor[i+1].remove(neighbor[i+1].get());
					return true;
				}else return false;	
		}
		return false;
	}
	
	public boolean move(Field moveTo) 
	{
		boolean ret = false;
		if(this.findDisk(Field.Direction.D, moveTo) == true) ret = true;
		if(this.findDisk(Field.Direction.U, moveTo) == true) ret = true;
		if(this.findDisk(Field.Direction.R, moveTo) == true) ret = true;
		if(this.findDisk(Field.Direction.L, moveTo) == true) ret = true;
		
		if(ret == true)moveTo.put(this);
		
		return ret;
	}
	
	public int hashCode() 
	{
		return 0;
	}
	
	public boolean equals(java.lang.Object obj) 
	{
		return (this == obj);
	}
	
	
}
