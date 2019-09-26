package ija.ija2018.homework1.board;

public class BoardField extends java.lang.Object implements Field
{

    public int row;
    public int col;

    private boolean definedDisk;
    private Disk disk;

    private Field[] around;

    public BoardField(int col, int row)
    {
        this.col = col;
        this.row = row;
        this.definedDisk = false;
        this.disk = null;
        this.around = new Field[8];
    }

    @Override
    public void addNextField(Field.Direction dirs, Field field)
    {
        int key = 0;
        for (Direction c : Direction.values()) {
            if (dirs == c) break;
            key++;
        }
        this.around[key] = field;
    }

    @Override
    public Disk get()
    {
        return this.disk;
    }

    @Override
    public Field nextField(Field.Direction dirs)
    {
        int key = 0;
        for (Direction c : Direction.values()) {
            if (dirs == c) break;
            key++;
        }
        return this.around[key];
    }

    @Override
    public boolean put(Disk disk)
    {
    	//ak je uz pole obsadene
        if (this.definedDisk) return false;
            
        this.definedDisk = true;
        this.disk = disk;
        return true;
    }

    @Override
    public boolean equals(Object obj)
    {
        if ((obj != null) && (obj instanceof BoardField) && (this.getClass() == obj.getClass())) {
            final BoardField other = (BoardField) obj;
            if ((other.row == this.row) && (other.col == this.col)) return true;
        }
        return false;
    }
    
    @Override
    public int hashCode()
    {
        return super.hashCode(); 
    }
    
    @Override
    public boolean remove(Disk disk)
    {
    	if (disk == null) return false;
    	if (this.disk.equals(disk) == false) return false;
        this.definedDisk = false;
        this.disk = null;
    	return true;
    }

	@Override
	public boolean isEmpty() {
    	if(disk == null) return true;
    	return false;
	}

}

