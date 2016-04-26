import java.math.BigInteger;
import java.util.function.Supplier;
import java.util.stream.Stream;
import java.util.stream.Collectors;

public class Fibonacci {
	private static final String name = Fibonacci.class.getSimpleName();

	public static void main(String []args) {
		if (1 != args.length) {
			System.err.println(String.format("The format is <java %s num>", name));
			System.exit(1);
		}

		// Get the number of fibonacci numbers the user wants
		int num = Integer.parseInt(args[0]);
		System.out.println(String.join(", ", Stream.generate(new FibSupplier())
			.limit(num)
			.map(f -> f.toString())
			.collect(Collectors.toList())));
	}

	private static class FibSupplier implements Supplier<BigInteger> {
		private BigInteger curr, next;

		public FibSupplier() {
			curr = BigInteger.ZERO;
			next = BigInteger.ONE;
		}

		public BigInteger get() {
			BigInteger ans = curr;
			curr = next;
			next = next.add(ans);
			return ans;
		}
	}
}