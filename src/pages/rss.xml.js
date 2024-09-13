import rss, {pagesGlobToRssItems} from '@astrojs/rss';

export async function GET(context) {
  return rss({
    title: 'natanhp.id',
    description: 'Thoughts and reflections on life\'s challenges, expressed through the written word. Discover my personal journey of learning and growth.',
    site: context.site,
    items: await pagesGlobToRssItems(
      import.meta.glob('./posts/*.{md,mdx}'),
    ),
    stylesheet: './rss/styles.xsl',
    customData: `<language>id-ID</language>`,
  });
}