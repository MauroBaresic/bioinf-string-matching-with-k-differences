package hr.fer.zesoi.bioinf.lvn;

import hr.fer.zesoi.bioinf.lvn.TripletList.Triplet;

/**
 * Class LVNAlgorithm represents an implementation of <i>Landau-Vishkin-Nussinov
 * k-differences string matching algorithm</i>.
 * <p>
 * As an input algorithm takes a text, pattern and number of allowed differences
 * (denoted by k).
 * </p>
 * <p>
 * Algorithm will find all the matches between text and pattern while allowing
 * matches to differ by k changes. Changes can be insertion, deletion and
 * swapping. Due to the nature of the algorithm, optimal (minimal) number of
 * changes will always be taken into account.
 * </p>
 * <p>
 * Output of the algorithm are all indices of the text from which the text
 * matches the pattern. Each index also comes with a number of differences
 * between the match (that number will always be lesser or equal to <i>k</i>).
 * </p>
 * 
 * @author Dino Pacandi
 * 
 */
public class LVNAlgorithm {

	/**
	 * Text whose parts are compared with the pattern.
	 */
	private char[] text;

	/**
	 * Pattern used in comparison.
	 */
	private char[] pattern;

	/**
	 * MaxLength table computed using the given pattern.
	 */
	private int[][] maxLength;

	/**
	 * Max allowed differences between pattern and text.
	 */
	private int k;

	/**
	 * First index denotes diagonal and second index denotes max allowed
	 * differences. Value stored on the indexed locations is the max row number
	 * which corresponds to the given constraints
	 */
	private LTable L;

	/**
	 * Peek memory usage.
	 */
	private long peekMemoryUsage = 0;

	/**
	 * Runtime.
	 */
	private Runtime runtime = Runtime.getRuntime();

	/**
	 * Default constructor for LVNAlgorithm.
	 * 
	 * @param text
	 *            Text which will be analyzed.
	 * @param pattern
	 *            Pattern which will be used in analysis and comparison with the
	 *            text.
	 * @param k
	 *            Max allowed differences between pattern and text.
	 */
	public LVNAlgorithm(String text, String pattern, int k) {
		this.text = text.toCharArray();
		Pattern tempPattern = new Pattern(pattern);
		this.pattern = tempPattern.getPattern();
		this.maxLength = tempPattern.getMaxLength();
		this.k = k;
	}

	/**
	 * Find all matching substrings between text and pattern whilst allowing
	 * k-differences between them.
	 */
	public void match() {
		int j = 0;
		int limit = text.length - pattern.length + k;
		int row, oldRow, maxLen;
		int c = 0, f = 0, maxJ = 0;
		TripletList Sij = null, newSij = null;
		L = new LTable(k);
		TList T, oldT;

		boolean breakOut = false;
		boolean skipOld = false;

		for (int i = 0; i <= limit; i++) {
			updatePeekMemoryUsage();
			// System.out.println("Processing index " + i);
			// [1]
			L.init();
			oldT = new TList(k);
			T = new TList(k);
			// [2]
			for (int e = 0; e <= k; e++) {
				oldT = T;
				T = new TList(k);
				for (int d = -e; d <= e; d++) {

					// [3]
					row = max3(L.get(d, e - 1) + 1, L.get(d - 1, e - 1), L.get(d + 1, e - 1) + 1);
					oldRow = row;

					if (row == L.get(d, e - 1) + 1) {
						T.set(d, oldT.get(d).getCopy());
						T.get(d).appendTriplet(i + row + d - 1, 0, 0);

					} else if (row == L.get(d - 1, e - 1)) {
						T.set(d, oldT.get(d + e).getCopy());
						T.get(d).appendTriplet(i + row + d - 1, 0, 0);
					} else if (row == L.get(d + 1, e - 1)) {
						T.set(d, oldT.get(d).getCopy());
					}

					// [4]
					while (row < pattern.length && (i + row + d) < text.length && (i + row + +d + 1) <= j) {
						// [4.1]
						c = 0;
						f = 0;
						if (Sij != null) {
							Triplet tempTriplet = Sij.findCoveringTriplet(i + row + d);
							c = tempTriplet.c;
							f = tempTriplet.f;
						}
						// [4.2]
						maxLen = maxLength[c][row];
						if (f > 1) {
							if (maxLen != f) {
								row += Math.min(maxLen, f);
								// GOTO 5
								skipOld = true;
								break;
							} else {
								row += f;
							}
						} else {
							if (text[i + row + d] != pattern[row]) {
								// GOTO 5
								skipOld = true;
								break;
							} else {
								row++;
							}
						}
					}
					updatePeekMemoryUsage();
					// [4.old]
					while (!skipOld && row < pattern.length && (i + row + d) < text.length
							&& pattern[row] == text[i + row + d]) {
						row++;
					}
					skipOld = false;
					// [5]
					L.set(d, e, row);

					if (row > oldRow) {
						T.get(d).appendTriplet(i + oldRow + d, oldRow, row - oldRow);
					}

					if (i + row + d > maxJ) {
						maxJ = i + row + d;
						newSij = T.get(d);
					}
					updatePeekMemoryUsage();
					// [6]
					if (L.get(d, e) == pattern.length) {
						;
						System.out.println(String.format("%10d%10d%10d", i, (i + row + d), e));
						// L.print();
						breakOut = true;
						break;
						// GOTO 7
					}
				}
				// equals to "goto [7]"
				if (breakOut == true) {
					breakOut = false;
					break;
				}
			}
			// [7]
			if (maxJ > j) {
				j = maxJ;
				Sij = newSij;
			}

		}
	}

	/**
	 * Getter for the peek memory usage.
	 * 
	 * @return Peek memory usage in bytes.
	 */
	public long getPeekMemoryUsage() {
		return peekMemoryUsage;
	};

	/**
	 * Sets the peek memory usage to 0.
	 */
	public void resetPeekMemoryUsage() {
		peekMemoryUsage = 0;
	}

	/**
	 * Method is used to determine which of the three given numbers is greatest.
	 * 
	 * @param a
	 *            First number.
	 * @param b
	 *            Second number.
	 * @param c
	 *            Third number.
	 * @return Greatest number.
	 */
	private int max3(int a, int b, int c) {
		int max = a > b ? a : b;
		max = max > c ? max : c;
		return max;
	}

	/**
	 * Method is used to update peek memory usage.
	 */
	private void updatePeekMemoryUsage() {
		Long memUsage = runtime.totalMemory() - runtime.freeMemory();
		peekMemoryUsage = Math.max(peekMemoryUsage, memUsage);
	}

	/**
	 * Class is used to represent 2D L table.<br>
	 * Table is accessed via two indexes:
	 * <ol>
	 * <li><i>d</i> - diagonal</li>
	 * <li><i>e</i> - number of allowed differences between text substring and
	 * pattern substring
	 * </ol>
	 * Element stored at the specified location is the row that was reached
	 * whilst taking into account the given constraints.
	 * 
	 * @author Dino Pacandi
	 * 
	 */
	public static class LTable {

		/**
		 * First index denotes diagonal and second index denotes max allowed
		 * differences. Value stored on the indexed locations is the max row
		 * number which corresponds to the given constraints
		 */
		private int[][] L;

		/**
		 * Maximum allowed number of differences between text and pattern.
		 */
		private int k;

		/**
		 * Default constructor for LTable.
		 * 
		 * @param k
		 *            Maximum allowed number of differences between text and
		 *            pattern.
		 */
		public LTable(int k) {
			// to cover border cases
			int temp = 2 * (k + 1) + 1;
			L = new int[temp][temp];
			this.k = k;
		}

		/**
		 * Returns the index of reached row for the specified diagonal and
		 * differences.
		 * 
		 * @param d
		 *            Diagonal.
		 * @param e
		 *            Number of differences.
		 * @return Reached row.
		 */
		public int get(int d, int e) {
			return L[d + k + 1][e + 2];
		}

		/**
		 * Method is used to set the value of reached row for the specified
		 * diagonal and number of differences to the given value.
		 * 
		 * @param d
		 *            Diagonal.
		 * @param e
		 *            Number of differences.
		 * @param val
		 *            New row that was reached.
		 */
		public void set(int d, int e, int val) {
			L[d + k + 1][e + 2] = val;
		}

		/**
		 * Method is used to initialize LTable.
		 */
		public void init() {
			int temp = 2 * (k + 1) + 1;
			L = new int[temp][temp];
			for (int d = -(k + 1); d <= (k + 1); d++) {
				int dAbs = Math.abs(d);
				this.set(d, dAbs - 2, Integer.MIN_VALUE);
				this.set(d, dAbs - 1, d < 0 ? dAbs - 1 : -1);
			}
		}

		/**
		 * Used for debugging.
		 */
		public void print() {
			for (int d = -(k + 1); d <= (k + 1); d++) {
				for (int e = 0; e <= k; e++) {
					System.out.print("(" + (d) + ", " + (e) + ") = " + this.get(d, e) + "\t");
				}
				System.out.println();
			}
		}

	}

	/**
	 * Class TList is used to store triplet lists.
	 * 
	 * @author Dino Pacandi
	 * 
	 */
	public static class TList {

		/**
		 * An array containing all the triplet lists for some <i>e</i> value.
		 */
		private TripletList[] T;

		/**
		 * Maximal number of allowed differences.
		 */
		private int k;

		/**
		 * Default constructor for TList.
		 * 
		 * @param k
		 *            Maximal number of allowed differences between text and
		 *            pattern.
		 */
		public TList(int k) {
			T = new TripletList[2 * k + 1];
			this.k = k;
			for (int i = 0; i < T.length; i++) {
				T[i] = new TripletList();
			}
		}

		/**
		 * Method is used to fetch the triplet list corresponding to the
		 * specified diagonal.
		 * 
		 * @param d
		 *            Diagonal.
		 * @return Triplet list.
		 */
		public TripletList get(int d) {
			return isOutOfBounds(d) ? new TripletList() : this.T[d + k];
		}

		/**
		 * Method is used to set the triplet list for the specified diagonal.
		 * 
		 * @param d
		 *            Diagonal.
		 * @param trpList
		 *            Triplet list corresponding to the given diagonal.
		 */
		public void set(int d, TripletList trpList) {
			if (false == isOutOfBounds(d)) {
				this.T[d + k] = trpList;
			}
		}

		/**
		 * Method is used to check whether the given diagonal exists or is here
		 * just to cover marginal cases.
		 * 
		 * @param d
		 *            Diagonal.
		 * @return <code>True</code> if diagonal exists, <code>false</code>
		 *         otherwise.
		 */
		private boolean isOutOfBounds(int d) {
			int index = d + k;
			boolean result = false;
			if (index < 0 || index >= T.length) {
				result = true;
			}
			return result;
		}

	}
}
