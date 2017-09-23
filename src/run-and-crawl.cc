/*
 * Run and Crawl - A student indie game
 * Copyright (C) 2017-2018 to authors see AUTHORS file
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <gf/Clock.h>
#include <gf/Color.h>
#include <gf/Event.h>
#include <gf/RenderWindow.h>
#include <gf/Window.h>

int main() {
    // initialization

    gf::Window window("Run and Crawl", { 1024, 768 });
    gf::RenderWindow renderer(window);

    // game loop
    gf::Clock clock;

    renderer.clear(gf::Color::White);

    while (window.isOpen()) {
        // Input
        gf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
                case gf::EventType::Closed:
                window.close();
                break;

                case gf::EventType::KeyPressed:

                break;

                default:
                break;
            }
        }

        float dt = clock.restart().asSeconds();

        // Update

        renderer.clear();

        // Render

        renderer.display();
    }

    return 0;
}
