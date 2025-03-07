import { glob } from 'astro/loaders';
import { defineCollection, z } from 'astro:content';

const postCollection = defineCollection({
	loader: glob({pattern: "*.md", base: "./src/pages/posts"}),
	schema: ({ image }) =>
		z.object({
			layout: z.string(),
			title: z.string(),
			pubDate: z.coerce.date(),
			description: z.string(),
			author: z.string(),
			excerpt: z.string(),
			image: z.object({
				src: image(),
				alt: z.string()
			}),
			tags: z.array(z.string())
		})
});

export const collections = {
	posts: postCollection
};
