package hr.fer.zesoi.bioinf.fasta;

import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;

/**
 * Class represents a single FASTA sequence. Sequence name, description and the
 * sequence itself is stored.
 * 
 * @author Dino Pacandi
 * 
 */
public class FASTASequence {

	/**
	 * Sequence identifier.
	 */
	private String name;

	/**
	 * Sequence description.
	 */
	private String description;

	/**
	 * Sequence.
	 */
	private String sequence;

	/**
	 * Default constructor for FASTA sequence.
	 * 
	 * @param name
	 *            Sequence name.
	 * @param description
	 *            Sequence description.
	 * @param sequence
	 *            Sequence itself.
	 */
	public FASTASequence(String name, String description, String sequence) {
		this.name = name;
		this.description = description;
		this.sequence = sequence;
	}

	/**
	 * Getter for sequence name.
	 * 
	 * @return Sequence name.
	 */
	public String getName() {
		return name;
	}

	/**
	 * Getter for the sequence description.
	 * 
	 * @return Sequence description.
	 */
	public String getDescription() {
		return description;
	}

	/**
	 * Getter for the sequence.
	 * 
	 * @return Sequence.
	 */
	public String getSequence() {
		return sequence;
	}

	public void writeSequenceAsFASTA(String fileName) {
		StringBuilder sb = new StringBuilder();
		sb.append(">").append(name).append(" ").append(description).append("\n");
		int strLen = sequence.length();
		for (int i = 0; i < strLen; i += 60) {
			sb.append(sequence.substring(i, Math.min(i + 60, strLen))).append("\n");
		}
		try {
			BufferedWriter writer = new BufferedWriter(new FileWriter(fileName));
			writer.write(sb.toString());
			writer.close();
		} catch (IOException e) {
			System.err.println("Failed to write sequence to FASTA file: " + fileName);
		}

	}

	/**
	 * Getter for sequence length.
	 * 
	 * @return Sequence length.
	 */
	public long getSequenceLength() {
		return sequence.length();
	}

	@Override
	public String toString() {
		StringBuilder sb = new StringBuilder();
		sb.append("Sequence name:  ").append(name).append("\n");
		sb.append("Sequence description:  ").append(description).append("\n");

		int strLen = sequence.length();
		for (int i = 0; i < strLen; i += 60) {
			sb.append(sequence.substring(i, Math.min(i + 60, strLen))).append("\n");
		}
		sb.deleteCharAt(sb.length() - 1);

		return sb.toString();
	}

}
