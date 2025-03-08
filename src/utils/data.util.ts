export const formatDate = (pubDate: string) => {
	var options: Intl.DateTimeFormatOptions = {
		weekday: 'short',
		year: 'numeric',
		month: 'long',
		day: 'numeric'
	};

	return new Date(pubDate).toLocaleDateString('id-ID', options);
};

export const sortBlogByDate = (
	a: { data: { pubDate: string | number | Date } },
	b: { data: { pubDate: string | number | Date } }
) => {
	const pubDateA = new Date(a.data.pubDate);
	const pubDateB = new Date(b.data.pubDate);
	if (pubDateA < pubDateB) {
		return 1;
	}
	if (pubDateA > pubDateB) {
		return -1;
	}
	return 0;
};
