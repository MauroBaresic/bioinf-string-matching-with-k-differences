package hr.fer.zesoi.bioinf.fasta;

/**
 * Class contains methods used for creating test files.
 * 
 * @author Dino Pacandi
 * 
 */
public class CreateTestFile {

	/**
	 * Used to create test file from the given sequence.
	 * 
	 * @param sequence
	 *            Sequence.
	 * @param fileName
	 *            Output file path.
	 * @param beginIndex
	 *            Starting index in the sequence.
	 * @param length
	 *            Length of the sub-sequence.
	 * @param name
	 *            Name of the sub-sequence.
	 * @param description
	 *            Description of the sequence.
	 */
	public static void createTestFile(FASTASequence sequence, String fileName, int beginIndex, int length,
			String name, String description) {
		int strLen = sequence.getSequence().length();

		if (beginIndex + length > strLen) {
			throw new IllegalArgumentException(
					"Cannot create test file when the specified substring is out of bounds.");
		}

		FASTASequence testSequence = new FASTASequence(name, description, sequence.getSequence().substring(
				beginIndex, beginIndex + length));
		testSequence.writeSequenceAsFASTA(fileName);
	}

}
