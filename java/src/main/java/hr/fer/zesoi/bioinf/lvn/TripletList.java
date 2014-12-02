package hr.fer.zesoi.bioinf.lvn;

import java.util.ArrayList;
import java.util.List;

/**
 * Class TripletList is used to store triplets containing information about
 * substring matches between some text and a pattern.
 * 
 * @author Dino Pacandi
 * 
 */
public class TripletList {

	/**
	 * List containing triplets.
	 */
	private List<Triplet> triplets;

	/**
	 * Default constructor for TripletList.
	 */
	public TripletList() {
		triplets = new ArrayList<>();
	}

	/**
	 * Appends triplet to the list of existing triplets.
	 * 
	 * @param triplet
	 *            New triplet.
	 */
	public void appendTriplet(Triplet triplet) {
		triplets.add(triplet);
	}

	/**
	 * Appends triplet to the list of existing triplets.
	 * 
	 * @param p
	 *            Starting index of the text substring.
	 * @param c
	 *            Starting index of the pattern substring.
	 * @param f
	 *            Length of the match.
	 */
	public void appendTriplet(int p, int c, int f) {
		appendTriplet(new Triplet(p, c, f));
	}

	/**
	 * Method is used to find (and calculate if necessary) the triplet that
	 * covers the user specified index.
	 * 
	 * @param index
	 *            Index whose coverage is required.
	 * @return Triplet that covers the given index.
	 */
	public Triplet findCoveringTriplet(int index) {
		Triplet trpResult = null;
		for (Triplet trp : triplets) {
			// index is normally covered by the triplet
			if (index == trp.p) {
				trpResult = trp.getCopy();
				break;
			} else if (index > trp.p && index <= trp.p + trp.f) {
				/*
				 * index is covered by the triplet but is not on the lower
				 * margin so triplet values need to recalculated
				 */
				int offset = index - trp.p;
				trpResult = new Triplet(trp.p + offset, trp.c + offset, trp.f - offset);
				break;
			} else {
				trpResult = new Triplet(0, 0, 0);
			}
		}
		return trpResult;
	}

	/**
	 * Method returns number of triplets contained within the list.
	 * 
	 * @return Number of triplets.
	 */
	public int getSize() {
		return triplets.size();
	}

	/**
	 * Method is used to get a copy of this list.
	 * 
	 * @return Copy of list.
	 */
	public TripletList getCopy() {
		TripletList result = new TripletList();
		result.triplets = new ArrayList<>(this.triplets);
		return result;
	}

	/**
	 * Class Triplet is used to store information about substring matches(of
	 * text and pattern).<br>
	 * Match is defined by three parameters:
	 * <ul>
	 * <li>p - starting index of the text substring</li>
	 * <li>c - starting index of the pattern substring (if c equals <i>zero</i>
	 * it means there is no match)</li>
	 * <li>f - length of the match (if f equals <i>zero</i> it means there is no
	 * match)</li>
	 * </ul>
	 * 
	 * @author Gossamer
	 * 
	 */
	public static class Triplet {

		/**
		 * Starting index of the text substring.
		 */
		public int p;

		/**
		 * Starting index of the pattern substring.
		 */
		public int c;

		/**
		 * Length of the match.
		 */
		public int f;

		/**
		 * Default constructor for triplet.<br>
		 * All triplet parameters are required.
		 * 
		 * @param p
		 *            Starting index of the text substring.
		 * @param c
		 *            Starting index of the pattern substring.
		 * @param f
		 *            Length of the match.
		 */
		public Triplet(int p, int c, int f) {
			this.p = p;
			this.c = c;
			this.f = f;
		}

		/**
		 * Returns the exact copy of this triplet.
		 * 
		 * @return Copy of triplet.
		 */
		public Triplet getCopy() {
			return new Triplet(p, c, f);
		}

	}
}
