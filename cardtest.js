function expand(card) {
    card.classList.toggle('profile--expanded');

    // If card is not expanded after toggle, add 'unexpanded' class
    if (!card.classList.contains('profile--expanded')) card.classList.toggle('profile--unexpanded');
    // Else if card is expanded after toggle and still contains 'unexpanded' class, remove 'unexpanded'
    else if (card.classList.contains('profile--expanded') && card.classList.contains('profile--unexpanded')) card.classList.toggle('profile--unexpanded');
}

function toggleTheme() {
    let docu = document.querySelector('html');

    docu.classList.toggle('light-theme');
    docu.classList.toggle('dark-theme');
}