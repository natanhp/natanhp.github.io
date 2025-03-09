import { glob } from 'astro/loaders';
import { defineCollection, z } from 'astro:content';

const blogCollection = defineCollection({
	loader: glob({ pattern: '*.{md,mdx}', base: './src/blogs' }),
	schema: ({ image }) =>
		z.object({
			title: z.string(),
			pubDate: z.coerce.date(),
			description: z.string(),
			author: z.string(),
			excerpt: z.string(),
			image: z.object({
				src: image().nullable(),
				alt: z.string().nullable(),
			}),
			tags: z.array(z.string())
		})
});

export const collections = {
	blog: blogCollection
};
