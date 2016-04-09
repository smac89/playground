public class Fibonacci {
	private static final String name = Fibonacci.class.getSimpleName();

	public static void main(String []args) {
		if (1 != args.length) {
			System.err.println(String.format("The format is <java %s num>", name));
			System.exit(1);
		}

		// Get the number of fibonacci numbers the user wants
		int num = Integer.parseInt(args[0]), fib1 = 0, fib2 = 1, tmp;
		while (num-- > 0) {
			System.out.println(fib1);
			tmp = fib1;
			fib1 = fib2;
			fib2 += tmp;
		}
	}
}