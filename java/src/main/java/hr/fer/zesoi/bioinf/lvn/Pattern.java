package hr.fer.zesoi.bioinf.lvn;

/**
 * <p>
 * Class Pattern represents pattern for LVN-algorithm. Also provided for each
 * pattern is a <i>MAXLEN</i> table which is used during the matching process of
 * LVN-algorithm.
 * </p>
 * <p>
 * MAXLEN[i][j] equals length of the matching substrings of the pattern
 * (substring starting at index <i>i</i> and substring starting at index
 * <i>j</i>).
 * </p>
 * 
 * @author Dino Pacandi
 * 
 */
public class Pattern {

	/**
	 * Pattern for text matching.
	 */
	private char[] pattern;

	/**
	 * MaxLength array.
	 */
	private int[][] maxLength = null;

	/**
	 * Default constructor for pattern.
	 * 
	 * @param pattern
	 *            Patetern used for text matching.
	 */
	public Pattern(String pattern) {
		this.pattern = pattern.toCharArray();
	}

	/**
	 * Method is used to return pattern as an array of characters.
	 * 
	 * @return Pattern.
	 */
	public char[] getPattern() {
		return pattern;
	}

	/**
	 * Method is used to return maxLength array for the pattern.
	 * 
	 * @return MaxLength 2D array.
	 */
	public int[][] getMaxLength() {
		if (maxLength == null) {
			createMaxLength();
		}
		int[][] temp = new int[maxLength.length][maxLength[0].length];
		for (int i = 0; i < maxLength.length; i++) {
			System.arraycopy(maxLength, 0, temp, 0, maxLength[0].length);
		}
		return maxLength;
	}

	/**
	 * Method is used to create MaxLength array for the pattern.
	 */
	public void createMaxLength() {
		maxLength = new int[pattern.length][pattern.length];
		for (int i = 0; i < pattern.length; i++) {
			for (int j = i; j < pattern.length; j++) {
				maxLength[i][j] = calculateF(i, j);
				// since the array is symmetric it is sufficient to calculate
				// only one half
				// of the 2D array, and simply mirror it in respect to the
				// matrix diagonal
				if (i != j) {
					maxLength[j][i] = maxLength[i][j];
				}
			}
		}
	}

	@Override
	public String toString() {
		return pattern.toString();
	}

	/**
	 * Method is used to generate string representation (user-readable) of
	 * MaxLength 2D array.
	 * 
	 * @return MaxLength array as string.
	 */
	public String maxLengthToString() {
		if (maxLength == null) {
			createMaxLength();
		}
		StringBuilder sb = new StringBuilder();
		for (int row = 0; row < maxLength.length; row++) {
			sb.append('[');
			for (int col = 0; col < maxLength.length; col++) {
				sb.append(String.format(" %3d ", maxLength[row][col]));
			}
			sb.append("]\n");
		}
		return sb.toString();
	}

	/**
	 * Method is used to calculate the length of matching characters in the
	 * pattern starting from i-th and j-th position. (i.e. p1p2p3 == p4p5p6 -> f
	 * = 3)
	 * 
	 * @param i
	 *            First index.
	 * @param j
	 *            Second index.
	 * @return Number of matching characters.
	 */
	private int calculateF(int i, int j) {
		int f = 0;
		int max = i > j ? i : j;
		while ((max + f) < pattern.length && pattern[i + f] == pattern[j + f]) {
			f++;
		}
		return f;
	}
}
