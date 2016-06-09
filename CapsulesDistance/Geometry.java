public class Geometry {
    private static final double EPS = 1e-6;

    private static double squareDistance(Point a, Point b) {
        return (a.getX() - b.getX()) * (a.getX() - b.getX())
                + (a.getY() - b.getY()) * (a.getY() - b.getY());
    }

    private static boolean equal(double a, double b) {
        return (Math.abs(a - b) < EPS);
    }

    private static boolean lessOrEqual(double a, double b) {
        return (a < b + EPS);
    }

    private static boolean less(double a, double b) {
        return (a + EPS < b);
    }

    private static boolean  greaterOrEqual(double a, double b) {
        return lessOrEqual(b, a);
    }

    public static boolean  greater(double a, double b) {
        return less(b, a);
    }

    public static double vectorMultiplication(Point a, Point b) {
        return (a.getX() * b.getY()) - (a.getY() * b.getX());
    }

    private static double scalarMultiplication(Point a, Point b) {
        return (a.getX() * b.getX()) + (a.getY() * b.getY());
    }

    private static boolean parallel(Point a, Point b) {
        return equal(vectorMultiplication(a, b), 0);
    }

    private static Point sumPoints(Point a, Point b) {
        Point result = new Point();
        result.setX(a.getX() + b.getX());
        result.setY(a.getY() + b.getY());
        return result;
    }

    private static Point subtractPoints(Point a, Point b) {
        Point result = new Point();
        result.setX(a.getX() - b.getX());
        result.setY(a.getY() - b.getY());
        return result;
    }

    private static void swap (Point a, Point b) {
        Point tmp = new Point();
        tmp.setX(a.getX());
        tmp.setY(a.getY());
        a.setX(b.getX());
        a.setY(b.getY());
        b.setX(tmp.getX());
        b.setY(tmp.getY());
    }

    private static void sort (Point a, Point b) {
        if (equal(a.getX(), b.getX())) {
            if (less(b.getY(), a.getY())) {
                swap(a, b);
            }
        } else if (less(b.getX(), a.getX())) {
            swap(a, b);
        }
    }

    private static Point solveLinearSystem(Equation p, Equation q) {
        double xDet = (((-p.getC())) * q.getB() - ((-q.getC())) * p.getB());
        double yDet = (p.getA() * (-q.getC())) - ((-p.getC()) * q.getA());
        double Det = (p.getA() * q.getB() - q.getA() * p.getB());

        double x = xDet / Det;
        double y = yDet / Det;

        Point result = new Point();
        result.setX(x);
        result.setY(y);
        return result;
    }

    private static boolean pointInsideBoundingBox(Point a, Point b, Point p) {
        if (lessOrEqual(Math.min(a.getX(), b.getX()), p.getX())
                && greaterOrEqual(Math.max(a.getX(), b.getX()), p.getX())
                && lessOrEqual(Math.min(a.getY(), b.getY()), p.getY())
                && greaterOrEqual(Math.max(a.getY(), b.getY()), p.getY())) {
            return true;
        } else {
            return false;
        }
    }

    private static double intervalsDistanceHelpFunction(Point intersecPoint,
                                                Point bbStart,
                                                Point bbEnd,
                                                Point start,
                                                Point end) {
        if (less(squareDistance(start, intersecPoint),
                squareDistance(end, intersecPoint))) {
            return pointVectorDistance(bbStart, bbEnd, start);
        } else {
            return pointVectorDistance(bbStart, bbEnd, end);
        }
    }

    private static Point projection(Point p, Point vect, Point start) {
        Point normal = new Point();
        normal.setX(-vect.getY());
        normal.setY(vect.getX());
        Equation eq1 = new Equation(vect, start);
        Equation eq2 = new Equation(normal, p);
        Point intersecPoint = solveLinearSystem(eq1, eq2);
        return intersecPoint;
    }

    private static double pointVectorDistance(Point start, Point end, Point p) {
        Point vect = subtractPoints(end, start);
        Point proj = projection(p, vect, start);
        if (pointInsideBoundingBox(end, start, proj)) {
            return Math.sqrt(squareDistance(proj, p));
        } else {
            return Math.sqrt(Math.min(squareDistance(p, start), squareDistance(p, end)));
        }
    }

    public static double distanceBetweenIntervals
            (Point start1, Point start2, Point end1, Point end2) {
        sort(start1, end1);
        sort(start2, end2);
        //assuming start < end according to xy order
        Point vect1 = subtractPoints(end1, start1);
        Point vect2 = subtractPoints(end2, start2);

        if (parallel(vect1, vect2)) {
            Point diff = subtractPoints(start2, start1);
            if (parallel(diff, vect1)) {
                if (lessOrEqual(diff.norm(), vect1.norm())) {
                    return 0;
                } else {
                    return Math.sqrt(Math.min(squareDistance(start1, end2),
                            squareDistance(start2, end1)));
                }
            } else {
                Point projStart = projection(start2, vect1, start1);
                Point projEnd = projection(end2, vect1, start1);
                if (pointInsideBoundingBox(start1, end1, projStart)) {
                    return Math.sqrt(squareDistance(projStart, start2));
                } else if (pointInsideBoundingBox(start1, end1, projEnd)) {
                    return Math.sqrt(squareDistance(projEnd, end2));
                } else {
                    return Math.sqrt(Math.min(Math.min(squareDistance(start1, end2), squareDistance(start1, start2)),
                            Math.min(squareDistance(start2, end1), squareDistance(end1,  end2))));
                }
            }
        } else {
            Equation eq1 = new Equation(vect1, start1);
            Equation eq2 = new Equation(vect2, start2);
            Point intersecPoint = solveLinearSystem(eq1, eq2);
            boolean insideBB2= pointInsideBoundingBox(end2, start2, intersecPoint);
            boolean insideBB1 = pointInsideBoundingBox(end1, start1, intersecPoint);
            if (insideBB1 && insideBB2) {
                return 0;
            } else {
                if (insideBB1) {
                    return intervalsDistanceHelpFunction(intersecPoint,
                            start1, end1, start2, end2);
                } else if (insideBB2) {
                    return intervalsDistanceHelpFunction(intersecPoint,
                            start2, end2, start1, end1);
                } else {
                    return Math.sqrt(Math.min(Math.min(squareDistance(start1, end2), squareDistance(start1, start2)),
                            Math.min(squareDistance(start2, end1), squareDistance(end1,  end2))));
                }
            }
        }
    }
}
