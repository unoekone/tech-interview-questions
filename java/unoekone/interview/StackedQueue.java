package unoekone.interview;

import java.util.Stack;

public class StackedQueue<T> {

private Stack<T> inStack = new Stack<T>();
private Stack<T> outStack = new Stack<T>();

public void enqueue(T val) {
	inStack.push(val);
}

public T dequeue() {
	if (outStack.isEmpty()) {
		while (!inStack.isEmpty()) {
			outStack.push(inStack.pop());
		}
	}

	return outStack.pop();
}

public static void main(String args[]) {

	StackedQueue<Integer> sq = new StackedQueue<Integer>();

	sq.enqueue(1);
	sq.enqueue(2);
	sq.enqueue(3);

	System.out.println("dequeue: " + sq.dequeue());
	System.out.println("dequeue: " + sq.dequeue());

	sq.enqueue(4);
	sq.enqueue(5);

	System.out.println("dequeue: " + sq.dequeue());
	System.out.println("dequeue: " + sq.dequeue());
	System.out.println("dequeue: " + sq.dequeue());
}

}
