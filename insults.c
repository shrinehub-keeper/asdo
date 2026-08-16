/*
 * Copyright (c) 2026 asdo contributors
 *
 * This file, except for the insults[] string literals noted below, is
 * free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your
 * option) any later version. See LICENSE.GPLv2 in the asdo source
 * tree, or <https://www.gnu.org/licenses/>, for the full license text.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * ---
 *
 * The bulk of the insults[] pool below is reproduced from the
 * "insults" plugin of the sudo project (plugins/sudoers/ins_classic.h,
 * ins_csops.h, ins_goons.h, ins_2001.h and ins_python.h), which is
 * Copyright (c) 1996-2004 Todd C. Miller <Todd.Miller@sudo.ws> and
 * distributed under the following ISC license (see LICENSE in the
 * asdo source tree for the identical license covering the rest of
 * the original OpenDoas codebase):
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * The final four entries in the pool (marked "asdo-specific insults"
 * below) are original to this file and covered by the GPLv2 grant
 * above, same as the rest of the file.
 */

#include "config.h"

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "openbsd.h"
#include "doas.h"

int insults_enabled = 0;
const char *custom_prompt = NULL;
const char *custom_prompt_err = NULL;

static const char *insults[] = {
	/* sudo "classic" insults */
	"Wrong!  You cheating scum!",
	"And you call yourself a Rocket Scientist!",
	"Where did you learn to type?",
	"Are you on drugs?",
	"My pet ferret can type better than you!",
	"You type like i drive.",
	"Do you think like you type?",
	"Your mind just hasn't been the same since the electro-shock, has it?",

	/* sudo "csops" insults */
	"Maybe if you used more than just two fingers...",
	"BOB says:  You seem to have forgotten your passwd, enter another!",
	"stty: unknown mode: doofus",
	"I can't hear you -- I'm using the scrambler.",
	"The more you drive -- the dumber you get.",
	"Listen, broccoli brains, I don't have time to listen to this trash.",
	"I've seen penguins that can type better than that.",
	"Have you considered trying to match wits with a rutabaga?",
	"You speak an infinite deal of nothing",

	/* sudo "goons" insults (The Goon Show) */
	"You silly, twisted boy you.",
	"He has fallen in the water!",
	"We'll all be murdered in our beds!",
	"You can't come in. Our tiger has got flu",
	"I don't wish to know that.",
	"What, what, what, what, what, what, what, what, what, what?",
	"You can't get the wood, you know.",
	"You'll starve!",
	"... and it used to be so popular...",
	"Pauses for audience applause, not a sausage",
	"Hold it up to the light --- not a brain in sight!",
	"Have a gorilla...",
	"There must be cure for it!",
	"There's a lot of it about, you know.",
	"You do that again and see what happens...",
	"Ying Tong Iddle I Po",
	"Harm can come to a young lad like that!",
	"And with that remarks folks, the case of the Crown vs yourself was proven.",
	"Speak English you fool --- there are no subtitles in this scene.",
	"You gotta go owwwww!",
	"I have been called worse.",
	"It's only your word against mine.",
	"I think ... err ... I think ... I think I'll go home",

	/* sudo "2001" insults (HAL 9000) */
	"Just what do you think you're doing Dave?",
	"It can only be attributed to human error.",
	"That's something I cannot allow to happen.",
	"My mind is going. I can feel it.",
	"Sorry about this, I know it's a bit silly.",
	"Take a stress pill and think things over.",
	"This mission is too important for me to allow you to jeopardize it.",
	"I feel much better now.",

	/* sudo "python" insults (Monty Python) */
	"That is no basis for supreme executive power!",
	"You empty-headed animal food trough wiper!",
	"I fart in your general direction!",
	"Your mother was a hamster and your father smelt of elderberries!",
	"You must cut down the mightiest tree in the forest... with... a herring!",
	"I wave my private parts at your aunties!",
	"He's not the Messiah, he's a very naughty boy!",
	"I wish to make a complaint.",
	"When you're walking home tonight, and some homicidal maniac comes after "
	    "you with a bunch of loganberries, don't come crying to me!",
	"This man, he doesn't know when he's beaten! He doesn't know when he's "
	    "winning, either. He has no... sort of... sensory apparatus...",
	"There's nothing wrong with you that an expensive operation can't prolong.",
	"I'm very sorry, but I'm not allowed to argue unless you've paid.",

	/* asdo-specific insults */
	"That's no good! Wrong password!",
	"I am the ultimate one... and you can't even remember your password.",
	"Chitty chitty chit chat... chit chat.",
	"alr brother add [core] to /etc/pacman.conf and pacman -S systemd if u "
	    "really are that dumb",
};

#define NINSULTS (sizeof(insults) / sizeof(insults[0]))

static const char *
pick_insult(void)
{
	static int seeded = 0;

	if (!seeded) {
		srandom((unsigned int)(time(NULL) ^ getpid()));
		seeded = 1;
	}

	return insults[random() % NINSULTS];
}

/*
 * Called whenever authentication fails, in place of the plain
 * "Authentication failed" message doas normally exits with.
 * doas/asdo only ever makes one authentication attempt, so this
 * always ends in exit(1), same as upstream OpenDoas.
 */
void __dead
authfail(void)
{
	const char *msg;

	if (insults_enabled)
		errx(1, "%s", pick_insult());

	msg = getenv("ASDO_ERR");
	if (!msg || !*msg)
		msg = custom_prompt_err;
	if (msg && *msg)
		errx(1, "%s", msg);

	errx(1, "Authentication failed");
}

/*
 * Safely expand %u (invoking user) and %h (hostname) in a prompt
 * template into buf. Never treats tmpl as a printf-style format
 * string, since it may come from an untrusted environment variable
 * (ASDO_PS1).
 */
static void
expand_prompt(char *buf, size_t bufsz, const char *tmpl, const char *user,
    const char *host)
{
	size_t bi = 0;
	size_t i;

	if (bufsz == 0)
		return;

	for (i = 0; tmpl[i] != '\0' && bi + 1 < bufsz; i++) {
		const char *sub = NULL;

		if (tmpl[i] != '%' || tmpl[i + 1] == '\0') {
			buf[bi++] = tmpl[i];
			continue;
		}

		i++;
		switch (tmpl[i]) {
		case 'u':
			sub = user;
			break;
		case 'h':
			sub = host;
			break;
		case '%':
			buf[bi++] = '%';
			continue;
		default:
			if (bi + 1 < bufsz)
				buf[bi++] = '%';
			if (bi + 1 < bufsz)
				buf[bi++] = tmpl[i];
			continue;
		}

		if (sub != NULL) {
			size_t slen = strlen(sub);
			size_t room = bufsz - bi - 1;
			size_t copy = slen < room ? slen : room;
			memcpy(buf + bi, sub, copy);
			bi += copy;
		}
	}
	buf[bi] = '\0';
}

/*
 * Build the password prompt into buf, honoring (in priority order)
 * the ASDO_PS1 environment variable, the "prompt" config directive,
 * and finally the standard doas prompt.
 */
void
build_prompt(char *buf, size_t bufsz, const char *user, const char *host)
{
	const char *tmpl;

	tmpl = getenv("ASDO_PS1");
	if (!tmpl || !*tmpl)
		tmpl = custom_prompt;
	if (!tmpl || !*tmpl)
		tmpl = "\rdoas (%u@%h) password: ";

	expand_prompt(buf, bufsz, tmpl, user, host);
}
