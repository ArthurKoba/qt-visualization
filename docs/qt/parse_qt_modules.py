#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
Парсер документации Qt модулей для извлечения полного списка классов с описаниями.
Загружает страницы модулей и парсит текст для получения всех классов и их описаний.
"""

import re
import sys
from urllib.request import urlopen
from urllib.error import URLError


def fetch_module_page(module_name):
    """Загружает страницу модуля Qt."""
    url = f"https://doc.qt.io/qt-6/{module_name}-module.html"
    try:
        print(f"Загрузка: {url}", file=sys.stderr)
        with urlopen(url, timeout=10) as response:
            return response.read().decode('utf-8')
    except URLError as e:
        print(f"Ошибка при загрузке {url}: {e}", file=sys.stderr)
        return None


def extract_classes_with_descriptions(html_content):
    """
    Извлекает классы с описаниями из HTML контента.
    Ищет паттерн: QClassName сразу за которым идет описание.
    """
    classes = []
    seen = set()
    
    # Удаляем HTML теги
    text = re.sub(r'<[^>]+>', '', html_content)
    
    # Ищем все классы с их описаниями
    # Паттерн: Q[A-Za-z0-9_:]+ (класс) затем текст до следующего класса
    # Используем non-greedy match для описания
    pattern = r'(Q[A-Za-z0-9_:]+)([^Q]*?)(?=Q[A-Za-z0-9_:]+|$)'
    
    matches = re.finditer(pattern, text)
    
    for match in matches:
        class_name = match.group(1).strip()
        description_raw = match.group(2).strip()
        
        # Проверяем, что это не дубликат
        if class_name not in seen:
            seen.add(class_name)
            
            # Берем только первую строку описания (до первого переноса)
            description = description_raw.split('\n')[0].strip()
            
            # Заменяем множественные пробелы на один
            description = re.sub(r'\s+', ' ', description)
            
            # Ограничиваем длину описания
            if len(description) > 200:
                description = description[:197] + "..."
            
            # Пропускаем пустые описания
            if description:
                classes.append({
                    'name': class_name,
                    'description': description,
                    'url': f"https://doc.qt.io/qt-6/{class_name.lower()}.html"
                })
    
    return classes


def format_markdown_table(classes):
    """Форматирует классы в таблицу Markdown."""
    if not classes:
        return "Классы не найдены."
    
    lines = ["| Класс | Описание | Ссылка |", "|-------|---------|--------|"]
    
    for cls in classes:
        class_name = cls['name']
        description = cls['description'] if cls['description'] else "—"
        # Экранируем специальные символы в описании для Markdown
        description = description.replace('|', '\\|').replace('\n', ' ')
        url = cls['url']
        lines.append(f"| `{class_name}` | {description} | [{class_name}]({url}) |")
    
    return "\n".join(lines)


def parse_module(module_name):
    """Парсит модуль Qt и возвращает список классов с описаниями."""
    html_content = fetch_module_page(module_name)
    if not html_content:
        return []
    
    classes = extract_classes_with_descriptions(html_content)
    return classes


def main():
    """Основная функция."""
    if len(sys.argv) < 2:
        print("Использование: python parse_qt_modules.py <module_name>", file=sys.stderr)
        print("Пример: python parse_qt_modules.py qtcore", file=sys.stderr)
        sys.exit(1)
    
    module_name = sys.argv[1]
    print(f"Парсинг модуля: {module_name}", file=sys.stderr)
    print("=" * 60, file=sys.stderr)
    
    classes = parse_module(module_name)
    
    if classes:
        print(f"Найдено классов: {len(classes)}\n", file=sys.stderr)
        print(format_markdown_table(classes))
    else:
        print("Классы не найдены.", file=sys.stderr)


if __name__ == "__main__":
    main()
