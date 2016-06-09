/**
 * Created by user on 09.06.2016.
 */

public class Point {
    private double x;
    private double y;

    public double getX() {
        return this.x;
    }
    public double getY() {
        return this.y;
    }
    public void setX(double x) {
        this.x = x;
    }
    public void setY(double y) {
        this.y = y;
    }

    public double norm() {
        return Math.sqrt(this.x * this.x + this.y * this.y);
    }
}